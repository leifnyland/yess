#include "PipeReg.h"

#ifndef PIPEREGARRAY_H
#define PIPEREGARRAY_H

//these can be used as indices into the array of PipeReg
#define FREG 0
#define DREG 1
#define EREG 2
#define MREG 3
#define WREG 4

//number of PipeRegisters
#define NUMPIPEREGS 5

class PipeRegArray
{
   private:
      PipeReg ** pregs;
   public:
      PipeRegArray();
      PipeReg * getFetchReg();
      PipeReg * getDecodeReg();
      PipeReg * getExecuteReg();
      PipeReg * getMemoryReg();
      PipeReg * getWritebackReg();
};
#endif
