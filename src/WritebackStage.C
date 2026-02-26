#include "PipeRegArray.h"
#include "WritebackStage.h"
#include "W.h"
#include "E.h"
#include "Status.h"
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
bool WritebackStage::doClockLow(PipeRegArray * pipeRegs)
{
	return pipeRegs->getWritebackReg()->get(W_STAT) != Status::SAOK;
}

/* doClockHigh
 *
 * applies the appropriate control signal to the F
 * and D register intances
 * 
 * @param: pipeRegs - array of the pipeline register (F, D, E, M, W instances)
*/
void WritebackStage::doClockHigh(PipeRegArray * pipeRegs)
{
	bool error = false;
	PipeReg * wreg = pipeRegs->getWritebackReg();
	rf->writeRegister(wreg->get(W_VALE), wreg->get(W_DSTE), error);
	rf->writeRegister(wreg->get(W_VALM), wreg->get(W_DSTM), error);
}



