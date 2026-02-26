#include <cstdint>
#ifndef INSTRUCTION_H
#define INSTRUCTION_H
class Instruction
{
   public:
      /* opcodes */
      static const int32_t IHALT = 0;
      static const int32_t INOP = 1;
      static const int32_t IRRMOVQ = 2;
      static const int32_t IIRMOVQ = 3;
      static const int32_t IRMMOVQ = 4;
      static const int32_t IMRMOVQ = 5;
      static const int32_t IOPQ = 6;
      static const int32_t IJXX = 7;
      static const int32_t ICMOVXX = 2;
      static const int32_t ICALL = 8;
      static const int32_t IRET = 9;
      static const int32_t IPUSHQ = 0xA;
      static const int32_t IPOPQ = 0xB;

      /* function codes */
      static const int32_t FNONE = 0;
      static const int32_t ADDQ = 0;
      static const int32_t SUBQ = 1;
      static const int32_t ANDQ = 2;
      static const int32_t XORQ = 3;

      static const int32_t UNCOND = 0;
      static const int32_t LESSEQ = 1;
      static const int32_t LESS = 2;
      static const int32_t EQUAL = 3;
      static const int32_t NOTEQUAL = 4;
      static const int32_t GREATEREQ = 5;
      static const int32_t GREATER = 6;
};
#endif
