#include "PipeRegField.h"
#include "F.h"

/*
 * F constructor
 *
 * initialize the F pipeline register 
*/
F::F()
{
   int32_t i;
   fields = new PipeRegField * [F_NUMFIELDS];
   numFields = F_NUMFIELDS;
   for (i = 0; i < numFields; i++)
   {
      fields[i] = new PipeRegField();
   }
}

/* 
 * dump
 *
 * outputs the current values of the F pipeline register
*/
void F::dump()
{
   uint64_t predPC = fields[F_PREDPC]->getOutput();
   dumpField("F: predPC: ", 3, predPC, true);
}
