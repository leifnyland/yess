#include "PipeReg.h"

#ifndef F_H
#define F_H
#define F_PREDPC 0
#define F_NUMFIELDS 1

//class to hold the F pipeline registers
class F : public PipeReg
{
   public:
      F();
      void normal();
      void dump();
};
#endif
