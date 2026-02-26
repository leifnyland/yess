#include "PipeRegArray.h"
#include "ExecuteStage.h"
#include "E.h"
#include "M.h"
#include "W.h"
#include "Instruction.h"
#include "Tools.h"
#include "Status.h"

/*
 * doClockLow
 *
 * Performs the Fetch stage combinational logic that is performed when
 * the clock edge is low.
 *
 * @param: pipeRegs - array of the pipeline register 
                      (F, D, E, M, W instances)
 */
bool ExecuteStage::doClockLow(PipeRegArray * pipeRegs)
{
	PipeReg * ereg = pipeRegs->getExecuteReg();
	PipeReg * mreg = pipeRegs->getMemoryReg();
	PipeReg * wreg = pipeRegs->getWritebackReg();
	uint64_t stat = ereg->get(E_STAT);
	uint64_t icode = ereg->get(E_ICODE);
	uint64_t ifun = ereg->get(E_IFUN);
	Stage::e_Cnd = e_cnd(icode, ifun);
	uint64_t valE = ereg->get(E_VALC);
	uint64_t valA = ereg->get(E_VALA);
	uint64_t dstE = ereg->get(E_DSTE);
	uint64_t dstM = ereg->get(E_DSTM);
	uint64_t W_stat = wreg->get(W_STAT);
	uint64_t m_stat = Stage::m_stat;


	calculateControlSignals(W_stat, m_stat);

	ALU(ereg);
	CC(ereg, wreg);
	valE = Stage::e_valE;
	dstE = ExecuteStage::e_dstE(ereg, dstE, Stage::e_Cnd);
	Stage::e_dstE = dstE;
	setMInput(mreg, stat, icode, Stage::e_Cnd, valE, valA, dstE, dstM);
	return false;
}

/* doClockHigh
 *
 * applies the appropriate control signal to the F
 * and D register intances
 * 
 * @param: pipeRegs - array of the pipeline register (F, D, E, M, W instances)
*/
void ExecuteStage::doClockHigh(PipeRegArray * pipeRegs)
{
	PipeReg * wreg = pipeRegs->getWritebackReg();
	PipeReg * mreg = pipeRegs->getMemoryReg();
	if (M_Bubble) {
		((M *)mreg)->bubble();
	}
	else
	{
		mreg->normal();
	}
	
}

/* setMInput
 * provides the input to potentially be stored in the M register
 * during doClockHigh
 *
 * @param: mreg - pointer to the M register instance
 * @param: stat - value to be stored in the stat pipeline register within M
 * @param: icode - value to be stored in the icode pipeline register within M
 * @param: cnd - value to be stored in M_Cnd
 * @param: valE - value to be stored in the valE pipeline register within M
 * @param: valA - value to be stored in the valA pipeline register within M
 * @param: dstE - value to be stored in the dstE pipeline register within M
 * @param: dstM - value to be stored in the dstM pipeline register within M
*/
void ExecuteStage::setMInput(PipeReg * mreg, uint64_t stat, uint64_t icode,
                           uint64_t cnd, uint64_t valE, uint64_t valA,
                           uint64_t dstE, uint64_t dstM)
{
   mreg->set(M_STAT, stat);
   mreg->set(M_ICODE, icode);
   mreg->set(M_CND, cnd);
   mreg->set(M_VALE, valE);
   mreg->set(M_VALA, valA);
   mreg->set(M_DSTE, dstE);
   mreg->set(M_DSTM, dstM);
}

/* aluA
 * 
 * calculates the A input to the alu
 * 
 * @param: ereg - E register object
 * 
 * @return: value for the A input to the ALU
*/
uint64_t ExecuteStage::aluA(PipeReg * ereg) 
{
	uint64_t i_code = ereg->get(E_ICODE);
	if(i_code == Instruction::IRRMOVQ || 
	   i_code == Instruction::IOPQ) return ereg->get(E_VALA);
	else if (i_code == Instruction::IIRMOVQ ||
			 i_code == Instruction::IRMMOVQ || 
			 i_code == Instruction::IMRMOVQ) return ereg->get(E_VALC);
	else if (i_code == Instruction::ICALL ||
			 i_code == Instruction::IPUSHQ) return -8;
	else if (i_code == Instruction::IRET ||
			 i_code == Instruction::IPOPQ) return 8;
	else return 0;
}

/* aluB
 * 
 * calculates the B input to the alu
 * 
 * @param: ereg - E register object
 * 
 * @return: value for the B input to the ALU
*/
uint64_t ExecuteStage::aluB(PipeReg * ereg) 
{
	uint64_t i_code = ereg->get(E_ICODE);
	if (i_code == Instruction::IRMMOVQ ||
		i_code == Instruction::IMRMOVQ ||
		i_code == Instruction::IOPQ ||
		i_code == Instruction::ICALL ||
		i_code == Instruction::IPUSHQ ||
		i_code == Instruction::IRET ||
		i_code == Instruction::IPOPQ) return ereg->get(E_VALB);
	// there is supposed to be another line here, but I think it is unecessary: E_icode in { IRRMOVQ, IIRMOVQ } : 0;
	else return 0;
}

/* alufun
 * 
 * calculates function to use in the ALU
 * 
 * @param: ereg - E register object
 * 
 * @return: function to use in the ALU
*/
uint64_t ExecuteStage::alufun(PipeReg * ereg) 
{
	uint64_t i_code = ereg->get(E_ICODE);
	if (i_code == Instruction::IOPQ) return ereg->get(E_IFUN);
	else return Instruction::ADDQ;
}

/* set_cc
 * 
 * decides if the condition codes need to be set
 * 
 * @param: ereg - E register object
 * 
 * @return: bool for if the codes need to be set
*/
bool ExecuteStage::set_cc(PipeReg * ereg, PipeReg * wreg) 
{
	uint64_t i_code = ereg->get(E_ICODE);
	uint64_t m_stat = Stage::m_stat;
	uint64_t W_stat = wreg->get(W_STAT);
	return (i_code == Instruction::IOPQ &&
	m_stat != Status::SADR &&
	m_stat != Status::SINS &&
	m_stat != Status::SHLT && 
	W_stat != Status::SADR &&
	W_stat != Status::SINS &&
	W_stat != Status::SHLT);
}

/* e_dstE
 * 
 * returns the destination register ID
 * 
 * @param: ereg - E register object
 * @param: dstE - destination calculated earlier
 * @param: cnd - 
 * 
 * @return: value in e_dstE
*/
uint64_t ExecuteStage::e_dstE(PipeReg * ereg, uint64_t dstE, uint64_t cnd) 
{
	uint64_t i_code = ereg->get(E_ICODE);
	if (i_code == Instruction::IRRMOVQ && (!cnd)) return RegisterFile::RNONE;
	else return dstE;
}

/* ALU
 * 
 * actually calculate the arithmetic opperation
 * 
 * @param: ereg - E register object
*/
void ExecuteStage::ALU(PipeReg * ereg) 
{
	uint64_t aluA = this->aluA(ereg);
	uint64_t aluB = this->aluB(ereg);
	uint64_t alufun = this->alufun(ereg);
	uint64_t answer;
	if      (alufun == Instruction::ADDQ) answer = aluA + aluB;
	else if (alufun == Instruction::SUBQ) answer = aluB - aluA;
	else if (alufun == Instruction::XORQ) answer = aluB ^ aluA;
	else if (alufun == Instruction::ANDQ) answer = aluB & aluA;
	Stage::e_valE = answer;
}

/* CC
 * 
 * set all the conditions codes
 * 
 * @param: ereg - E register object
 */
void ExecuteStage::CC(PipeReg * ereg, PipeReg * wreg) 
{
	bool error;
	if (set_cc(ereg, wreg)) {
		uint64_t aluA = this->aluA(ereg);
		uint64_t aluB = this->aluB(ereg);
		uint64_t alufun = this->alufun(ereg);
		cc->setConditionCode(Stage::e_valE == 0, ConditionCodes::ZF, error);
		cc->setConditionCode(Tools::sign(Stage::e_valE), ConditionCodes::SF, error);
		bool valASign = Tools::sign(aluA);
		bool valBSign = Tools::sign(aluB);
		bool answerSign = Tools::sign(Stage::e_valE);
		bool overflowFlag = 0;
		overflowFlag |= (alufun == Instruction::ADDQ && (valASign == valBSign && valASign != answerSign));
		overflowFlag |= (alufun == Instruction::SUBQ && (valASign != valBSign && valBSign != answerSign));
		cc->setConditionCode(overflowFlag, ConditionCodes::OF, error);
	}
}

/* e_cnd
 * looks at the flags and returns a value based on the calling function and instruction
 * 
 * @param: icode - instruction code
 * @param: ifun - function code
 * 
 * @return: e_cnd
 */
uint64_t ExecuteStage::e_cnd(uint64_t icode, uint64_t ifun) 
{
	bool error;
	bool sf = cc->getConditionCode( ConditionCodes::SF, error);
	bool of = cc->getConditionCode( ConditionCodes::OF, error);
	bool zf = cc->getConditionCode( ConditionCodes::ZF, error);
	uint64_t e_cnd;
	if (icode == Instruction::IJXX || icode == Instruction::ICMOVXX)
	{
		if (ifun == Instruction::UNCOND)
		{
			e_cnd = 1;
		}
		else if (ifun == Instruction::LESSEQ)
		{
			e_cnd = (sf ^ of) | zf;
		}
		else if (ifun == Instruction::LESS)
		{
			e_cnd = (sf ^ of);
		}
		else if (ifun == Instruction::EQUAL)
		{
			e_cnd = zf;
		}
		else if (ifun == Instruction::NOTEQUAL)
		{
			e_cnd = !zf;
		}
		else if (ifun == Instruction::GREATER)
		{
			e_cnd = !(sf ^ of) && !zf;
		}
		else //ifun == GREATEREQ
		{
			e_cnd = !(sf ^ of);
		}
	}
	else
	{
		return 0;
	}
	return e_cnd;
}

/* M_Bubble
 * 
 * decides if the M reg needs to be bubbled
 * @param: W_stat
 * 
 * @return: M reg needs to be bubbled or not
 */
bool ExecuteStage::M_bubble(uint64_t W_stat, uint64_t m_stat)
{
	return (m_stat == Status::SADR ||
	m_stat == Status::SINS ||
	m_stat == Status::SHLT ||
	W_stat == Status::SADR ||
	W_stat == Status::SINS ||
	W_stat == Status::SHLT);
}

/* calculateControlSignals
 * Sets the M_Bubble control signals
 *
 * @param: W_stat
*/
void ExecuteStage::calculateControlSignals(uint64_t W_stat, uint64_t m_stat)
{
	M_Bubble = M_bubble(W_stat, m_stat);
}