#include "PipeRegArray.h"
#include "Memory.h"
#include "RegisterFile.h"
#include "ConditionCodes.h"
#include "StageArray.h"

#ifndef CPU_H
#define CPU_H
//Driver class for the yess simulator
class CPU
{
   private:
      PipeRegArray * pipeRegs;
      StageArray * stages;
      Memory * mem;
      RegisterFile * rf;
      ConditionCodes * cc;
   public:
      CPU();
      void run();
      bool doClockLow();
      void doClockHigh();
      void dumpPipeRegs();
};
#endif
