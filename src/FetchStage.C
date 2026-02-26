//TODO add more #includes as you need them
#include <cstdint>
#include "PipeRegArray.h"
#include "PipeReg.h"
#include "Memory.h"
#include "FetchStage.h"
#include "Instruction.h"
#include "RegisterFile.h"
#include "Status.h"
#include "F.h"
#include "D.h"
#include "E.h"
#include "M.h"
#include "W.h"
#include "Tools.h"

/*
 * doClockLow
 *
 * Performs the Fetch stage combinational logic that is performed when
 * the clock edge is low.
 *
 * @param: pipeRegs - array of the pipeline register 
                      (F, D, E, M, W instances)
 */
bool FetchStage::doClockLow(PipeRegArray * pipeRegs)
{
   PipeReg * freg = pipeRegs->getFetchReg();
   PipeReg * dreg = pipeRegs->getDecodeReg();
   PipeReg * ereg = pipeRegs->getExecuteReg();
   PipeReg * mreg = pipeRegs->getMemoryReg();
   PipeReg * wreg = pipeRegs->getWritebackReg();
   bool mem_error = false;
   uint64_t f_icode = Instruction::INOP, ifun = Instruction::FNONE;
   uint64_t rA = RegisterFile::RNONE, rB = RegisterFile::RNONE;
   uint64_t valC = 0, valP = 0, stat = 0, predPC = freg->get(F_PREDPC);
   bool needvalC = false;
   bool needregId = false;
   uint64_t D_icode = dreg->get(D_ICODE);
   uint64_t E_icode = ereg->get(E_ICODE);
   uint64_t E_dstM = ereg->get(E_DSTM);
   uint64_t M_icode = mreg->get(M_ICODE);
   uint64_t m_Cnd = mreg->get(M_CND);
   uint64_t M_valA = mreg->get(M_VALA);
   uint64_t W_icode = wreg->get(W_ICODE);
   uint64_t W_valM= wreg->get(W_VALM);

   calculateControlSignals(D_icode, E_icode, M_icode, E_dstM);

   //TODO 
   //select PC value and read byte from memory
   //set icode and ifun using byte read from memory
   //uint64_t f_pc =  .... call your select pc function
	uint64_t f_pc = FetchStage::selectPC(M_icode, m_Cnd, M_valA, W_icode, W_valM, predPC);
	uint64_t temp = mem->getByte(f_pc, mem_error);
	f_icode = Tools::getBits(temp, 4, 7); //mask makes sure to not pick up any weirdness
	ifun = Tools::getBits(temp, 0, 3);

	if (mem_error)
	{
		f_icode = Instruction::INOP;
		ifun = Instruction::FNONE;
	}

   //In order to calculate the address of the next instruction,
   //you'll need to know whether this current instruction has an
   //immediate field and a register byte. (Look at the instruction encodings.)
   //needvalC =  .... call your need valC function
   //needregId = .... call your need regId function
    needvalC = needValC(f_icode);
	needregId = needRegIDs(f_icode);
	if (needregId)
	{
		uint64_t regByte = mem->getByte(f_pc + 1, mem_error);
		getRegIDs(f_pc, regByte, rA, rB);
	}
	
	if (needvalC)
	{
		valC = buildValC(mem, f_pc, mem_error, f_icode);
	}

   //determine the address of the next sequential function
   //valP = ..... call your PC increment function 
    valP = PCincrement(f_pc, needregId, needvalC);

   //calculate the predicted PC value
   //predPC = .... call your function that predicts the next PC   
   predPC = predictPC(f_icode, valC, valP);
   //set the input for the PREDPC pipe register field in the F register
   freg->set(F_PREDPC, predPC);


	if (mem_error)
	{
		f_icode = Instruction::INOP;
		ifun = RegisterFile::RNONE;
	}
	
   //status of this instruction
	uint64_t instr_valid = FetchStage::instr_valid(f_icode);
	stat = FetchStage::f_stat(mem_error, instr_valid, f_icode);

   //set the inputs for the D register
   setDInput(dreg, stat, f_icode, ifun, rA, rB, valC, valP);
   return false;
}

/* doClockHigh
 *
 * applies the appropriate control signal to the F
 * and D register intances
 * 
 * @param: pipeRegs - array of the pipeline register (F, D, E, M, W instances)
*/
void FetchStage::doClockHigh(PipeRegArray * pipeRegs)
{
   PipeReg * freg = pipeRegs->getFetchReg();  
   PipeReg * dreg = pipeRegs->getDecodeReg();
   if (!F_stall) freg->normal();
   if (D_bubble) ((D *)dreg)->bubble();
   else if (!D_stall) dreg->normal();
}

/* setDInput
 * provides the input to potentially be stored in the D register
 * during doClockHigh
 *
 * @param: dreg - pointer to the D register instance
 * @param: stat - value to be stored in the stat pipeline register within D
 * @param: icode - value to be stored in the icode pipeline register within D
 * @param: ifun - value to be stored in the ifun pipeline register within D
 * @param: rA - value to be stored in the rA pipeline register within D
 * @param: rB - value to be stored in the rB pipeline register within D
 * @param: valC - value to be stored in the valC pipeline register within D
 * @param: valP - value to be stored in the valP pipeline register within D
*/
void FetchStage::setDInput(PipeReg * dreg, uint64_t stat, uint64_t icode,
                           uint64_t ifun, uint64_t rA, uint64_t rB,
                           uint64_t valC, uint64_t valP)
{
   dreg->set(D_STAT, stat);
   dreg->set(D_ICODE, icode);
   dreg->set(D_IFUN, ifun);
   dreg->set(D_RA, rA);
   dreg->set(D_RB, rB);
   dreg->set(D_VALC, valC);
   dreg->set(D_VALP, valP);
}
//Write your selectPC, needRegIds, needValC, PC increment, and predictPC methods
//Remember to add declarations for these to FetchStage.h

// Here is the HCL describing the behavior for some of these methods. 


/* selectPC
 * selects the correct PC counter value based on the current instruction
 * during doClockHigh
 *
 * @param: M_icode
 * @param: m_Cnd
 * @param: M_valA
 * @param: W_icode
 * @param: W_valM
 * @param: predPC
 * 
 * @return: the correct next PC value
*/
uint64_t FetchStage::selectPC(uint64_t M_icode, uint64_t m_Cnd, uint64_t M_valA, uint64_t W_icode, uint64_t W_valM, uint64_t predPC)
{
	if (M_icode == Instruction::IJXX && !m_Cnd) 
	{
		return M_valA;
	}
	else if (W_icode == Instruction::IRET)
	{
		return W_valM;
	}
	else 
	{
		return predPC;
	}
}

/* needRegIDs
 * detemines if register IDs are needed based on the current intruction
 *
 * @param: f_icode - instruction code of the insruction in the f reg
 * 
 * @return: true/false for if the regIDs are needed
 */
bool FetchStage::needRegIDs (uint64_t f_icode)
{
	//bool need_regids = f_icode in { IRRMOVQ, IOPQ, IPUSHQ, IPOPQ, IIRMOVQ, IRMMOVQ, IMRMOVQ };
	return f_icode == Instruction::IRRMOVQ || 
		   f_icode == Instruction::IOPQ || 
		   f_icode == Instruction::IPUSHQ || 
		   f_icode == Instruction::IPOPQ || 
		   f_icode == Instruction::IIRMOVQ || 
		   f_icode == Instruction::IRMMOVQ || 
		   f_icode == Instruction::IMRMOVQ;
}

/* needValC
 * detemines if valC is needed based on the current instruction code
 *
 * @param: f_icode - instruction code of the insruction in the f reg
 * 
 * @return: true/false valC is needed
 */
bool FetchStage::needValC (uint64_t f_icode)
{
	//bool need_valC = f_icode in { IIRMOVQ, IRMMOVQ, IMRMOVQ, IJXX, ICALL };
	return f_icode == Instruction::IIRMOVQ|| 
		   f_icode == Instruction::IRMMOVQ || 
		   f_icode == Instruction::IMRMOVQ || 
		   f_icode == Instruction::IJXX || 
		   f_icode == Instruction::ICALL;
}

/* predictPC
 * detemines pc for jump and call instructions as if they are taken
 *
 * @param: f_icode - instruction code of the insruction in the f reg
 * @param: f_valC - valC for the instruction
 * @param: f_valP - old pc counter as if not jumped/called
 * 
 * @return: correct pc based on instruction
 */
uint64_t FetchStage::predictPC(uint64_t f_icode, uint64_t f_valC, uint64_t f_valP)
{
	return (f_icode == Instruction::IJXX || f_icode == Instruction::ICALL) ? f_valC:f_valP;
}

/* PCincrement
 * increments the pc based on the size of the current instruction
 *
 * @param: f_pc - old/current pc
 * @param: needsRegIDs - output of needsRegIDs
 * @param: needsValC - output needsValC
 * 
 * @return: value of the next sequential instruction
 */
uint64_t FetchStage::PCincrement(uint64_t f_pc, bool needsRegIds, bool needsValC)
{
	if (needsRegIds && needsValC)
	{
		return f_pc + 10;
	}
	else if (needsRegIds)
	{
		return f_pc + 2;
	}
	else if (needsValC)
	{
		return f_pc + 9;
	}
	else
	{
		return f_pc + 1;
	}
}

/* getRegIDs
 * gets the values (IDs) the registers
 *
 * @param: f_pc - old/current pc
 * @param: regByte - register byte of the instruction
 * @param: rA - value to store the ID of register A
 * @param: rB - value to store the ID of register B
 */
void FetchStage::getRegIDs(uint64_t f_pc, uint64_t regByte, uint64_t &rA, uint64_t &rB) 
{

	rA = Tools::getBits(regByte, 4, 7);
	rB = Tools::getBits(regByte, 0, 3);	
}

/* PCincrement
 * extracts valC into a returnable value
 *
 * @param: mem - memory object pointer
 * @param: f_pc - current PC value
 * 
 * @return: valC
 */
uint64_t FetchStage::buildValC(Memory * mem, uint64_t f_pc, bool mem_error, uint64_t f_icode)
{
	uint64_t valC = 0;
	for (uint64_t i = 0; i < 8; i++)
	{
		valC += ((uint64_t)mem->getByte(f_pc + 1 + needRegIDs(f_icode) + i, mem_error) << 8 * i);
	}
	return valC;
}

/* F_stall
 * Sees if a stall is necessary when a value is moved from mem and then immediately used
 *
 * @param: D_icode - instruction code in the D reg
 * @param: E_icode - instruction code in the E reg
 * @param: M_icode - instruction code in the M reg
 * @param: E_dstM - reg destination in the E reg
 * 
 * @return: stall is/is not needed for F reg
 */
bool FetchStage::f_stall(uint64_t D_icode, uint64_t E_icode, uint64_t M_icode, uint64_t E_dstM)
{
	return((E_icode == Instruction::IMRMOVQ ||
			E_icode == Instruction::IPOPQ) &&
		   (E_dstM == Stage::d_srcA ||
		    E_dstM == Stage::d_srcB)) ||
		   (D_icode == Instruction::IRET ||
			E_icode == Instruction::IRET ||
			M_icode == Instruction::IRET);
}

/* D_stall
 * Sees if a stall is necessary when a value is moved from mem and then immediately used
 *
 * @param: E_icode - instruction code in the E reg
 * @param: E_dstM - reg destination in the E reg
 * 
 * @return: stall is/is not needed for D reg
 */
bool FetchStage::d_stall(uint64_t E_icode, uint64_t E_dstM)
{
	return (E_icode == Instruction::IMRMOVQ ||
			E_icode == Instruction::IPOPQ) &&
		   (E_dstM == Stage::d_srcA ||
		    E_dstM == Stage::d_srcB);
}
/* D_bubble
 * Sees if a bubble is necessary when a value is moved from mem and then immediately used
 *
 * @param: E_icode - instruction code in the E reg
 * @param: E_dstM - reg destination in the E reg
 * 
 * @return: bubble is/is not needed for D reg
 */
bool FetchStage::d_bubble(uint64_t D_icode, uint64_t E_icode, uint64_t M_icode, uint64_t E_dstM, bool e_Cnd)
{
	return (E_icode == Instruction::IJXX &&
		   !e_Cnd) ||
		   
		(!((E_icode == Instruction:: IMRMOVQ ||
		   	E_icode == Instruction:: IPOPQ) &&
		   (E_dstM == Stage::d_srcA ||
		    E_dstM == Stage::d_srcB)) &&
		   (D_icode == Instruction::IRET ||
			E_icode == Instruction::IRET ||
			M_icode == Instruction::IRET));
}

/* calculateControlSignals
 * Sets the F_stall and D_stall control signals
 *
 * @param: D_icode - instruction code in the D reg
 * @param: E_icode - instruction code in the E reg
 * @param: M_icode - instruction code in the M reg
 * @param: E_dstM - reg destination in the E reg
*/
void FetchStage::calculateControlSignals(uint64_t D_icode, uint64_t E_icode, uint64_t M_icode, uint64_t E_dstM)
{
	F_stall = f_stall(D_icode, E_icode, M_icode, E_dstM);
	D_stall = d_stall(E_icode, E_dstM);	
	bool e_Cnd = Stage::e_Cnd;
	D_bubble = d_bubble(D_icode, E_icode, M_icode, E_dstM, e_Cnd);
}

/* instr_valid
 * 
 * Checks to see if the instruction is a valid i_code
 * 
 * @param: f_icode - the instruction code in freg
 * 
 * @return: true if the icode is valid
 */
bool FetchStage::instr_valid(uint64_t f_icode)
{
	return (f_icode == Instruction::INOP ||
	f_icode == Instruction::IHALT   	 ||
	f_icode == Instruction::IRRMOVQ 	 ||
	f_icode == Instruction::IIRMOVQ 	 ||
	f_icode == Instruction::IRMMOVQ 	 ||
	f_icode == Instruction::IMRMOVQ 	 ||
	f_icode == Instruction::IOPQ    	 ||
	f_icode == Instruction::IJXX    	 ||
	f_icode == Instruction::ICALL   	 ||
	f_icode == Instruction::IRET    	 ||
	f_icode == Instruction::IPUSHQ  	 ||
	f_icode == Instruction::IPOPQ);
}

/* f_stat
 * returns the value to be stored in stat field
 * 
 * @param: mem_error - error from mem
 * @param: instr_valid - is the current istruction valid
 * @param: f_icode - current istruction code
 * 
*/
uint64_t FetchStage::f_stat(bool mem_error, bool instr_valid, uint64_t f_icode)
{
	if (mem_error)
	{
		return Status::SADR;
	}
	if (!instr_valid)
	{
		return Status::SINS;
	}
	if (f_icode == Instruction::IHALT)
	{
		return Status::SHLT;
	}
	return Status::SAOK;
}
