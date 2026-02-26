#ifndef CONDITIONCODESTESTER_H
#define CONDITIONCODESTESTER_H

//class to test the ConditionCodes class
class ConditionCodesTester
{
   private:
      ConditionCodes * cc;
   public:
      ConditionCodesTester();
      void doConditionCodesTests();
      void doSetClearConditionCodeTests(int32_t ccNum);
      void doErrorConditionCodeTests(int32_t ccNum);
};
#endif
