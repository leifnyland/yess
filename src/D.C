#include "Instruction.h"
#include "RegisterFile.h"
#include "PipeRegField.h"
#include "D.h"
#include "Status.h"

/*
 * D constructor
 *
 * initialize the D pipeline register
 */
D::D()
{
   int32_t i;
   fields = new PipeRegField * [D_NUMFIELDS];
   numFields = D_NUMFIELDS;
   for (i = 0; i < numFields; i++)
   {
       fields[i] = new PipeRegField();
   }
   bubble();
}

/*
 * Sets the field of the D register so that
 * the stored instruction is a NOP
 */
void D::bubble()
{
   fields[D_STAT]->setState(Status::SAOK);
   fields[D_ICODE]->setState(Instruction::INOP);
   fields[D_IFUN]->setState(Instruction::FNONE);
   fields[D_RA]->setState(RegisterFile::RNONE);
   fields[D_RB]->setState(RegisterFile::RNONE);
   fields[D_VALC]->setState();
   fields[D_VALP]->setState();
}

/* 
 * dump
 *
 * outputs the current values of the D pipeline register
*/
void D::dump()
{
   dumpField("D: stat: ", 1, fields[D_STAT]->getOutput(), false);
   dumpField(" icode: ", 1, fields[D_ICODE]->getOutput(), false);
   dumpField(" ifun: ", 1, fields[D_IFUN]->getOutput(), false);
   dumpField(" rA: ", 1, fields[D_RA]->getOutput(), false);
   dumpField(" rB: ", 1, fields[D_RB]->getOutput(), false);
   dumpField(" valC: ", 16, fields[D_VALC]->getOutput(), false);
   dumpField(" valP: ", 3, fields[D_VALP]->getOutput(), true);
}
