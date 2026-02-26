#include <string>
#include "PipeRegField.h"

#ifndef PIPEREG_H
#define PIPEREG_H

//base class for the F, D, E, M, W pipeline registers
class PipeReg
{
   public:
      //dump method is implemented in the classes that descend
      //from PipeReg
      //
      //dump is abstract
      //virtual makes it polymorphic 
      virtual void dump() = 0;
      uint64_t get(int32_t which);
      void set(int32_t which, uint64_t value);
      void normal();
   protected:
      PipeRegField ** fields;
      int32_t numFields;
      void dumpField(std::string label, int32_t width, uint64_t value, bool nl);
};
#endif
