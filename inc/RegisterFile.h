
#ifndef REGISTERFILE_H
#define REGISTERFILE_H

class RegisterFile 
{

   public:
      static const int size = 15;
      //Most of these you won't use, but they are here for
      //completeness
      static const int rax = 0;
      static const int rcx = 1;
      static const int rdx = 2;
      static const int rbx = 3;
      static const int rsp = 4;
      static const int rbp = 5;
      static const int rsi = 6;
      static const int rdi = 7;
      static const int r8 = 8;
      static const int r9 = 9;
      static const int r10 = 0xa;
      static const int r11 = 0xb;
      static const int r12 = 0xc;
      static const int r13 = 0xd;
      static const int r14 = 0xe;
      static const int RNONE = 0xf;
   private:
      static RegisterFile * regInstance;
      RegisterFile();
      uint64_t reg[RegisterFile::size];
   public:
      static RegisterFile * getInstance();      
      uint64_t readRegister(int32_t regNumber, bool & error);
      void writeRegister(uint64_t value, int32_t regNumber, 
                        bool & error);
      void dump();
}; 

#endif
