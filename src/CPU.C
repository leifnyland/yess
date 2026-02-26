/* 
 * CPU class
 *
 * The CPU class contains objects to represent the FetchStage, DecodeStage,
 * ExecuteStage, MemoryStage, and Writeback Stages. These classes contain the
 * methods to simulate the combinational logic performed by the PIPE machine.
 * In addition, the CPU class contains F, D, E, M, and W objects
 * to represent the pipelined registers that provide the input and
 * accept the output of these stages.
*/
#include <iomanip>
#include <iostream>
#include "CPU.h"
#include "PipeRegArray.h"
#include "StageArray.h"
#include "Memory.h"
#include "ConditionCodes.h"
#include "RegisterFile.h"

/*
 * CPU constructor
 *
 * creates instances of each data member
*/
CPU::CPU()
{
   /* PIPE stages */
   stages = new StageArray();

   /* pipelined registers */
   pipeRegs = new PipeRegArray();

   mem = Memory::getInstance();
   cc = ConditionCodes::getInstance();
   rf = RegisterFile::getInstance();
}

/* 
 * run
 * 
 * CPU the stages of the PIPE machine until a halt is executed.
*/
void CPU::run()
{
   int32_t cycle = 0;
   bool stop = false;

   while (!stop)
   {
      stop = doClockLow();
      doClockHigh();

      /* dump the values of the pipelined registers, Condition Codes, */
      /* Register File, and Memory */
      std::cout << "\nAt end of cycle " << std::dec 
         << cycle << ":" << std::endl;
      dumpPipeRegs();
      cc->dump();
      rf->dump();
      mem->dump();
      cycle++;
   }
}

/*
 * doClockLow
 *
 * When the clock is low, the stages use their inputs
 * to calculate the outputs.
*/
bool CPU::doClockLow()
{
   bool stop;

   //going through the stages in reverse order helps to
   //simulate the parallel behavior of the hardware
   stop = stages->getWritebackStage()->doClockLow(pipeRegs);
   stages->getMemoryStage()->doClockLow(pipeRegs);
   stages->getExecuteStage()->doClockLow(pipeRegs);
   stages->getDecodeStage()->doClockLow(pipeRegs);
   stages->getFetchStage()->doClockLow(pipeRegs);
   return stop;
}

/*
 * doClockHigh
 *
 * When the clock rises, the pipelined registers are updated.
 * The register file is also updated.
 * The code to update the pipelined registers is in the
 * pipe stage class.
*/
void CPU::doClockHigh()
{
   //get the WritebackStage to update the register file
   stages->getWritebackStage()->doClockHigh(pipeRegs);

   //get the MemoryStage to update the W register
   stages->getMemoryStage()->doClockHigh(pipeRegs);

   //get the ExecuteStage to update the M register
   stages->getExecuteStage()->doClockHigh(pipeRegs);

   //get the DecodeStage to update the E register
   stages->getDecodeStage()->doClockHigh(pipeRegs);

   //get the FetchStage to update the F and D registers
   stages->getFetchStage()->doClockHigh(pipeRegs);
}

/*
 * dumpPipeRegs
 *
 * Display the contents of the pipelined registers
*/
void CPU::dumpPipeRegs()
{
   pipeRegs->getFetchReg()->dump();
   pipeRegs->getDecodeReg()->dump();
   pipeRegs->getExecuteReg()->dump();
   pipeRegs->getMemoryReg()->dump();
   pipeRegs->getWritebackReg()->dump();
}
