#ifndef CONDITIONCODES_H
#define CONDITIONCODES_H

class ConditionCodes 
{
   private:
      static ConditionCodes * ccInstance;
      ConditionCodes();
      uint64_t codes;
   public:
      //Be sure to use these const variables to select the bit
      //Don't use a "magic" number
      //I made these strange values so that things may go wrong
      //if you don't use them
      static const int SF = 7;
      static const int OF = 3;
      static const int ZF = 2;
      static ConditionCodes * getInstance();      
      bool getConditionCode(int32_t ccNum, bool & error);
      void setConditionCode(bool value, int32_t ccNum, 
                            bool & error);
      void dump();
}; 

#endif
