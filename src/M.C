#include <cstdint>
#include "RegisterFile.h"
#include "Instruction.h"
#include "PipeRegField.h"
#include "M.h"
#include "Status.h"

/*
 * M constructor
 *
 * initialize the M pipeline register
*/
M::M()
{
   int32_t i;
   fields = new PipeRegField * [M_NUMFIELDS];
   numFields = M_NUMFIELDS;
   for (i = 0; i < numFields; i++)
   {
       fields[i] = new PipeRegField();
   }
   bubble();
}

/*
 * Set the fields of the M register so it contains
 * a NOP
 */
void M::bubble()
{
   int32_t i;
   for (i = 0; i < numFields; i++)
   {
       fields[i]->setState();
   }
   fields[M_STAT]->setState(Status::SAOK);
   fields[M_ICODE]->setState(Instruction::INOP);
   fields[M_DSTE]->setState(RegisterFile::RNONE);
   fields[M_DSTM]->setState(RegisterFile::RNONE);
}

/* 
 * dump
 *
 * outputs the current values of the M pipeline register
*/
void M:: dump()
{
   uint64_t stat = fields[M_STAT]->getOutput();
   uint64_t icode = fields[M_ICODE]->getOutput();
   uint64_t Cnd = fields[M_CND]->getOutput();
   uint64_t valE = fields[M_VALE]->getOutput();
   uint64_t valA = fields[M_VALA]->getOutput();
   uint64_t dstE = fields[M_DSTE]->getOutput();
   uint64_t dstM = fields[M_DSTM]->getOutput();
   dumpField("M: stat: ", 1, stat, false);
   dumpField(" icode: ", 1, icode, false);
   dumpField(" Cnd: ", 1, Cnd, false);
   dumpField(" valE: ", 16, valE, false);
   dumpField(" valA: ", 16, valA, false);
   dumpField(" dstE: ", 1, dstE, false);
   dumpField(" dstM: ", 1, dstM, true);
}
