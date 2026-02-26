#include "PipeReg.h"

#ifndef M_H
#define M_H

#define M_STAT 0
#define M_ICODE 1
#define M_CND 2
#define M_VALE 3
#define M_VALA 4
#define M_DSTE 5
#define M_DSTM 6
#define M_NUMFIELDS 7


//class to hold M pipeline registers
class M : public PipeReg
{
   public:
      M();
      void bubble();
      void dump();
};
#endif
