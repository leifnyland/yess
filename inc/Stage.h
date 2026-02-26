#include <cstdint>

#include "PipeRegArray.h"
#include "Memory.h"
#include "RegisterFile.h"
#include "ConditionCodes.h"

#ifndef STAGE_H
#define STAGE_H

class Stage
{
   protected:
      //make these static so there is just one instance
      //shared among all descendant classes
      static Memory * mem;
      static RegisterFile * rf;
      static ConditionCodes * cc;

      //values calculated by the DecodeStage that are needed by the FetchStage
      static uint64_t d_srcA;
      static uint64_t d_srcB;

      //values calculated by the ExecuteStage
      static uint64_t e_dstE;
      static uint64_t e_valE; 
      static uint64_t e_Cnd;

      //values calculated by the MemoryStage
      static uint64_t m_stat;
      static uint64_t m_valM;
   public:
      Stage();
      //abstract methods implemented in the descendant classes
      //virtual makes these methods polymorphic       
      virtual bool doClockLow(PipeRegArray * pipeRegs) = 0;
      virtual void doClockHigh(PipeRegArray * pipeRegs) = 0;
};
#endif


