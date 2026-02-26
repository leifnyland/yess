#include "PipeReg.h"

#ifndef W_H
#define W_H

#define W_STAT 0
#define W_ICODE 1
#define W_VALE 2
#define W_VALM 3
#define W_DSTE 4
#define W_DSTM 5
#define W_NUMFIELDS 6

//class to hold the W pipeline registers
class W : public PipeReg
{
   public:
      W();
      void bubble();
      void dump();
};
#endif
