#include "PipeReg.h"
#ifndef D_H
#define D_H

//class to hold the D pipeline registers
//indices into the inherited fields array
#define D_STAT 0
#define D_ICODE 1
#define D_IFUN 2
#define D_RA 3
#define D_RB 4
#define D_VALC 5
#define D_VALP 6
#define D_NUMFIELDS 7
class D : public PipeReg
{
   public:
      D();
      void bubble();
      void dump();
};
#endif
