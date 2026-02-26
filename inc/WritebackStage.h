#include "PipeRegArray.h"
#include "Stage.h"

#ifndef WRITEBACKSTAGE_H
#define WRITEBACKSTAGE_H
class WritebackStage: public Stage
{
   private:
      //TODO: provide declarations for new methods
   public:
      //These are the only methods called outside of the class
      bool doClockLow(PipeRegArray * pipeRegs);
      void doClockHigh(PipeRegArray * pipeRegs);
};
#endif
