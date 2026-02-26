#include "PipeRegArray.h"
#include "PipeReg.h"
#include "DecodeStage.h"
#include "D.h"
#include "E.h"
#include "M.h"
#include "W.h"
#include "Instruction.h"
#include "RegisterFile.h"

/*
 * doClockLow
 *
 * Performs the Fetch stage combinational logic that is performed when
 * the clock edge is low.
 *
 * @param: pipeRegs - array of the pipeline register 
                      (F, D, E, M, W instances)
 */
bool DecodeStage::doClockLow(PipeRegArray * pipeRegs)
{
	PipeReg * dreg = pipeRegs->getDecodeReg();
	PipeReg * ereg = pipeRegs->getExecuteReg();
	PipeReg * mreg = pipeRegs->getMemoryReg();   //for forwarding
	PipeReg * wreg = pipeRegs->getWritebackReg();
	uint64_t stat = dreg->get(D_STAT);
	uint64_t icode = dreg->get(D_ICODE);
	uint64_t ifun = dreg->get(D_IFUN);
	uint64_t valC = dreg->get(D_VALC);
	//need declarations for D_rA, D_rB, rsp, and rnone
	uint64_t D_rA = dreg->get(D_RA);
	uint64_t D_rB = dreg->get(D_RB);
	uint64_t rsp = RegisterFile::rsp;
	uint64_t rnone = RegisterFile::RNONE;

	uint64_t dstE = getd_dstE(icode, D_rB, rsp, rnone);
	uint64_t dstM = getd_dstM(icode, D_rA, rnone);
	uint64_t srcA = Stage::d_srcA = getd_srcA(icode, D_rA, rsp, rnone);
	uint64_t srcB = Stage::d_srcB = getd_srcB(icode, D_rB, rsp, rnone);
	uint64_t valP = dreg->get(D_VALP);
	uint64_t M_dstM = mreg->get(M_DSTM);
	uint64_t M_dstE = mreg->get(M_DSTE);
	uint64_t M_valE = mreg->get(M_VALE);
	uint64_t W_dstM = wreg->get(W_DSTM);
	uint64_t W_valM = wreg->get(W_VALM);
	uint64_t W_dstE = wreg->get(W_DSTE);
	uint64_t W_valE = wreg->get(W_VALE);

	uint64_t E_icode = ereg->get(E_ICODE);
    uint64_t E_dstM = ereg->get(E_DSTM);

    calculateControlSignals(E_icode, E_dstM);

	//the below will need looked over in future labs
	bool error = false;
	uint64_t d_rvalA = rf->readRegister(srcA, error);
	uint64_t d_rvalB = rf->readRegister(srcB, error);
	//These execute after, because they rely on srcA and srcB
	
	uint64_t valA = getd_valA(icode, srcA, d_rvalA, valP, M_dstM, M_dstE, M_valE, W_dstM, W_valM, W_dstE, W_valE);  //- incomplete for lab 7 - this comment can probably be disregarded
	uint64_t valB = getd_valB(srcB, d_rvalB, M_dstM, M_dstE, M_valE, W_dstM, W_valM, W_dstE, W_valE);  //- incomplete for lab 7 - this comment can probably be disregarded

	setEInput(ereg, stat, icode, ifun, valC, valA, valB, dstE, dstM, srcA, srcB);
	return false;
}

/* doClockHigh
 *
 * applies the appropriate control signal to the F
 * and D register intances
 * 
 * @param: pipeRegs - array of the pipeline register (F, D, E, M, W instances)
*/
void DecodeStage::doClockHigh(PipeRegArray * pipeRegs)
{
	//Need to make sure it works after I get step 2 done for lab 10
	PipeReg * ereg = pipeRegs->getExecuteReg();
	if (DecodeStage::E_bubble) ((E *)ereg)->bubble();
	else ereg->normal();
}

/* setEInput
 * provides the input to potentially be stored in the E register
 * during doClockHigh
 *
 * @param: ereg - pointer to the E register instance
 * @param: stat - value to be stored in the stat pipeline register within E
 * @param: icode - value to be stored in the icode pipeline register within E
 * @param: ifun - value to be stored in the ifun pipeline register within E
 * @param: valC - value to be stored in the valC pipeline register within E
 * @param: valA - value to be stored in the valA pipeline register within E
 * @param: valB - value to be stored in the valB pipeline register within E
 * @param: dstM - value to be stored in the dstM pipeline register within E
 * @param: srcA - value to be stored in the srcA pipeline register within E
 * @param: srcB - value to be stored in the srcB pipeline register within E
*/
void DecodeStage::setEInput(PipeReg * ereg, uint64_t stat, uint64_t icode,
                           uint64_t ifun, uint64_t valC, uint64_t valA,
                           uint64_t valB, uint64_t dstE, uint64_t dstM,
						   uint64_t srcA, uint64_t srcB)
{
   ereg->set(E_STAT, stat);
   ereg->set(E_ICODE, icode);
   ereg->set(E_IFUN, ifun);
   ereg->set(E_VALC, valC);
   ereg->set(E_VALA, valA);
   ereg->set(E_VALB, valB);
   ereg->set(E_DSTE, dstE);
   ereg->set(E_DSTM, dstM);
   ereg->set(E_SRCA, srcA);
   ereg->set(E_SRCB, srcB);
}

/* get_srcA
 * gets the correct value for srcA
 *
 * @param: icode - curent instruction code
 * @param: D_rA - register A value
 * @param: rsp - value at the top of the stack
 * @param: rnone - value for no register
 * 
 * @return: value to put into d_srcA
 */
uint64_t DecodeStage::getd_srcA(uint64_t icode, uint64_t D_rA, uint64_t rsp, uint64_t rnone) 
{
	if (icode == Instruction::IRRMOVQ ||
		icode == Instruction::IRMMOVQ ||
		icode == Instruction::IOPQ ||
		icode == Instruction::IPUSHQ)
	{
		return D_rA;
	}
	if (icode == Instruction::IPOPQ ||
		icode == Instruction::IRET)
	{
		return rsp;
	}
	return rnone;
}

/* get_srcB
 * gets the correct value for srcB
 *
 * @param: icode - curent instruction code
 * @param: D_rB - register B value
 * @param: rsp - value at the top of the stack
 * @param: rnone - value for no register
 * 
 * @return: value to put into d_srcB
 */
uint64_t DecodeStage::getd_srcB(uint64_t icode, uint64_t D_rB, uint64_t rsp, uint64_t rnone) 
{
	if (icode == Instruction::IOPQ ||
		icode == Instruction::IRMMOVQ ||
		icode == Instruction::IMRMOVQ)
	{
		return D_rB;
	}
	if (icode == Instruction::IPUSHQ ||
		icode == Instruction::IPOPQ ||
		icode == Instruction::ICALL ||
		icode == Instruction::IRET)
	{
		return rsp;
	}
	return rnone;
}

/* get_dstE
 * gets the correct value for the destination register
 *
 * @param: icode - curent instruction code
 * @param: D_rB - register B value to use as the dst
 * @param: rsp - value at the top of the stack
 * @param: rnone - value for no register
 * 
 * @return: register destination
 */
uint64_t DecodeStage::getd_dstE(uint64_t icode, uint64_t D_rB, uint64_t rsp, uint64_t rnone) 
{
	if (icode == Instruction::IRRMOVQ ||
		icode == Instruction::IIRMOVQ ||
		icode == Instruction::IOPQ)
	{
		return D_rB;
	}
	if (icode == Instruction::IPUSHQ ||
		icode == Instruction::IPOPQ ||
		icode == Instruction::ICALL ||
		icode == Instruction::IRET)
	{
		return rsp;
	}
	return rnone;
}

/* get_dstM
 * gets the correct location for the memory destination
 *
 * @param: icode - curent instruction code
 * @param: D_rA - memory address stored in rA
 * @param: rsp - value at the top of the stack
 * @param: rnone - value for no register
 * 
 * @return: memory destination
 */
uint64_t DecodeStage::getd_dstM(uint64_t icode, uint64_t D_rA, uint64_t rnone) 
{
	if (icode == Instruction::IMRMOVQ ||
		icode == Instruction::IPOPQ)
	{
		return D_rA;
	}
	return rnone;
}

/* getd_valA
 * Sel + FwdA
 * gets the correct value for d_valA using forwarding
 *
 * @param: icode - curent instruction code
 * @param: srcA
 * @param: d_rvalA
 * @param: valP
 * @param: M_dstM
 * @param: M_dstE
 * @param: M_valE
 * @param: W_dstM
 * @param: W_valM
 * @param: W_dstE
 * @param: W_valE
 * 
 * @return: valA
 */
//Forwarding Method Sel+FwdA
uint64_t DecodeStage::getd_valA(uint64_t icode, uint64_t srcA, uint64_t d_rvalA, uint64_t valP, 
								uint64_t M_dstM, uint64_t M_dstE, uint64_t M_valE, uint64_t W_dstM,
								uint64_t W_valM, uint64_t W_dstE, uint64_t W_valE) 
{
	if (icode == Instruction::ICALL ||
		icode == Instruction::IJXX)
	{
		return valP;
	}
	if (srcA == RegisterFile::RNONE)
	{
		return 0;
	}
	if (srcA == e_dstE) {
		return e_valE;
	}
	if (srcA == M_dstM)
	{
		return m_valM;
	}
	if (srcA == M_dstE)
	{
		return M_valE;
	}
	if (srcA == W_dstM)
	{
		return W_valM;
	}
	if (srcA == W_dstE)
	{
		return W_valE;
	}
	return d_rvalA;
}

/* getd_valB
 * FwdB
 * gets the correct value for d_valB using forwarding
 *
 * @param: srcB
 * @param: d_rvalB
 * @param: M_dstM
 * @param: M_dstE
 * @param: M_valE
 * @param: W_dstM
 * @param: W_valM
 * @param: W_dstE
 * @param: W_valE
 * 
 * @return: valB
 */
//Forwarding Method FwdB
uint64_t DecodeStage::getd_valB(uint64_t srcB, uint64_t d_rvalB, uint64_t M_dstM, uint64_t M_dstE,
								uint64_t M_valE, uint64_t W_dstM, uint64_t W_valM, 
								uint64_t W_dstE, uint64_t W_valE) 
{
	if (srcB == RegisterFile::RNONE)
	{
		return 0;
	}
	if (srcB == Stage::e_dstE) {
		return Stage::e_valE;
	}
	if (srcB == M_dstM)
	{
		return m_valM;
	}
	if (srcB == M_dstE)
	{
		return M_valE;
	}
	if (srcB == W_dstM)
	{
		return W_valM;
	}
	if (srcB == W_dstE)
	{
		return W_valE;
	}
	return d_rvalB;
}

/* e_bubble
 * Sees if a bubble is necessary when a value is moved from mem and then immediately used
 *
 * @param: E_icode - instruction code in the E reg
 * @param: E_dstM - reg destination in the E reg
 * 
 * @return: bubble is/is not needed for E reg
 */
bool DecodeStage::e_bubble(uint64_t E_icode, uint64_t E_dstM, bool e_Cnd)
{
	return (E_icode == Instruction::IJXX &&
		   !e_Cnd) ||
		  ((E_icode == Instruction::IMRMOVQ ||
			E_icode == Instruction::IPOPQ) &&
		   (E_dstM == Stage::d_srcA ||
			E_dstM == Stage::d_srcB));

}

/* calculateControlSignals
 * Sets the E_bubble control signal
 *
 * @param: E_icode - instruction code in the E reg
 * @param: E_dstM - reg destination in the E reg
*/
void DecodeStage::calculateControlSignals(uint64_t E_icode, uint64_t E_dstM)
{
	DecodeStage::E_bubble = e_bubble(E_icode, E_dstM, Stage::e_Cnd);
}


