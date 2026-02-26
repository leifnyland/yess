#include "PipeRegArray.h"
#include "MemoryStage.h"
#include "M.h"
#include "W.h"
#include "Instruction.h"
#include "Status.h"
//TODO update makefile with all includes in each stage

/*
 * doClockLow
 *
 * Performs the Fetch stage combinational logic that is performed when
 * the clock edge is low.
 *
 * @param: pipeRegs - array of the pipeline register 
                      (F, D, E, M, W instances)
 */
bool MemoryStage::doClockLow(PipeRegArray * pipeRegs)
{
	PipeReg * mreg = pipeRegs->getMemoryReg();
	PipeReg * wreg = pipeRegs->getWritebackReg();
	uint64_t stat = Stage::m_stat = mreg->get(M_STAT);
	uint64_t icode = mreg->get(M_ICODE);
	uint64_t valE = mreg->get(M_VALE);
	uint64_t valM = 0;
	uint64_t dstE = mreg->get(M_DSTE);
	uint64_t dstM = mreg->get(M_DSTM);

	// Data Memory logic
	bool mem_error_read = false;
	bool mem_error_write = false;
	uint64_t tempAddr = mem_addr(icode, valE, mreg->get(M_VALA));
	if (mem_read(icode))
	{
		valM = m_valM = mem->getLong(tempAddr, mem_error_read);
	}
	if (mem_write(icode))
	{
		mem->putLong(mreg->get(M_VALA), tempAddr, mem_error_write);
	}
	if (mem_error_read || mem_error_write)
	{
		Stage::m_stat = stat = Status::SADR;
	}
	
	setWInput(wreg, stat, icode, valE, valM, dstE, dstM);
	return false;
}

/* doClockHigh
 *
 * applies the appropriate control signal to the F
 * and D register intances
 * 
 * @param: pipeRegs - array of the pipeline register (F, D, E, M, W instances)
*/
void MemoryStage::doClockHigh(PipeRegArray * pipeRegs)
{
	pipeRegs->getWritebackReg()->normal();
}

/* setWInput
 * provides the input to potentially be stored in the W register
 * during doClockHigh
 *
 * @param: wreg - pointer to the W register instance
 * @param: stat - value to be stored in the stat pipeline register within W
 * @param: icode - value to be stored in the icode pipeline register within W
 * @param: valE - value to be stored in the valE pipeline register within W
 * @param: valM - value to be stored in the valM pipeline register within W
 * @param: dstE - value to be stored in the dstE pipeline register within W
 * @param: dstM - value to be stored in the dstM pipeline register within W
*/
void MemoryStage::setWInput(PipeReg * wreg, uint64_t stat, uint64_t icode,
                           uint64_t valE, uint64_t valM, uint64_t dstE,
                           uint64_t dstM)
{
   wreg->set(W_STAT, stat);
   wreg->set(W_ICODE, icode);
   wreg->set(W_VALE, valE);
   wreg->set(W_VALM, valM);
   wreg->set(W_DSTE, dstE);
   wreg->set(W_DSTM, dstM);
}

/* mem_addr
 * 
 * calculates memory address
 * 
 * @param: M_icode - instruction code
 * @param: M_valE - 
 * @param: M_valA - 
 * 
 * @return: memory address 
*/
// Addr component
uint64_t MemoryStage::mem_addr(uint64_t M_icode, uint64_t M_valE, uint64_t M_valA)
{
	if (M_icode == Instruction::IRMMOVQ ||
		M_icode == Instruction::IPUSHQ ||
		M_icode == Instruction::ICALL ||
		M_icode == Instruction::IMRMOVQ)
	{
		return M_valE;
	}

	else if (M_icode == Instruction::IPOPQ ||
			 M_icode == Instruction::IRET)
	{
		return M_valA;
	}
	else return 0;
}

/* mem_read
 * 
 * checks if memory should be read
 * 
 * @param: M_icode - instruction code
 * 
 * @return: memory address 
*/
// Mem Read component
bool MemoryStage::mem_read(uint64_t M_icode)
{
	return M_icode == Instruction::IMRMOVQ ||
		   M_icode == Instruction::IPOPQ ||
		   M_icode == Instruction::IRET;
}

/* mem_write
 * 
 * checks if memory should be written
 * 
 * @param: M_icode - instruction code
 * 
 * @return: memory address 
*/
// Mem Write component
bool MemoryStage::mem_write(uint64_t M_icode)
{
	return M_icode == Instruction::IRMMOVQ ||
		   M_icode == Instruction::IPUSHQ ||
		   M_icode == Instruction::ICALL;
}


