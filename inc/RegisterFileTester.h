
#ifndef REGISTERFILETESTER_H
#define REGISTERFILETESTER_H

//class to test the RegisterFile class
class RegisterFileTester
{
   private:
      void doClearRegisterTests();
      void doReadWriteRegisterTests();
      void doReadWriteErrorTests();
      RegisterFile * rf;
   public:
      RegisterFileTester();
      void doRegisterFileTests();
};

#endif

