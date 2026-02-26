#include <cstdint>

#ifndef PIPEREGFIELD_H
#define PIPEREGFIELD_H

//class to represent a single
//pipeline register
class PipeRegField
{
   private: 
      uint64_t input;   //current input to the register
      uint64_t state;   //current state (output)
   public:
      //default state is 0 if none is provided
      PipeRegField(int32_t state = 0); 
      void setInput(uint64_t input);
      uint64_t getOutput();
      void normal();
      void setState(int32_t state = 0);
};
#endif

  
