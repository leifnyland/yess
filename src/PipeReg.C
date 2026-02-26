#include <iostream>
#include <iomanip>
#include <cstdint>
#include <assert.h>
#include "PipeReg.h"


/*
 * return the value of a field in the pipe register
 */
uint64_t PipeReg::get(int32_t which)
{
   assert(which >= 0 && which < numFields);
   return fields[which]->getOutput();
}

/*
 * set the value of a field in the pipe register
 */
void PipeReg::set(int32_t which, uint64_t value)
{
   assert(which >= 0 && which < numFields);
   fields[which]->setInput(value);
}

/*
 * apply the normal signal to each field in the pipe register
 */
void PipeReg::normal()
{
   int32_t i;
   for (i = 0; i < numFields; i++)
      fields[i]->normal();
}

/* dumpField
 * Outputs a string and a uint64_t using the indicated width and padding with 0s.
 * If newline is true, a newline is output afterward.
 *
 * @param: fieldname - string to output; width used is the size of the string
 * @param: width - width in which to output the uint64_t
 * @param: fieldvalue - uint64_t that is output in width columns and padded with 0s
 * @param: newline - if true a newline is output after the fieldname and field value
 */
void PipeReg::dumpField(std::string fieldname, int32_t width, uint64_t fieldvalue, bool newline)
{
   std::cout << fieldname << std::hex << std::setw(width) << std::setfill('0') << fieldvalue;
   if (newline) std::cout << std::endl;
}   
