#include <iostream>
#include <assert.h>
#include "ConditionCodes.h"
#include "ConditionCodesTester.h"

/** 
 * ConditionCodesTester constructor
 * Initializes the ConditionCodes instance 
 *
*/
ConditionCodesTester::ConditionCodesTester()
{
   this->cc = ConditionCodes::getInstance();
}

/** 
 * doConditionCodesTests
 * Tests ConditionCodes methods
 *
*/
void ConditionCodesTester::doConditionCodesTests()
{
   //make sure there is a ConditionCodes instance
   assert(this->cc != NULL);

   //Call getInstance again to make sure it returns the
   //same instance.  If this fails, you didn't set
   //up the singleton class properly.
   assert(this->cc == ConditionCodes::getInstance());

   //make sure the codes variable has been initialized to 0
   bool code, error;
   code = this->cc->getConditionCode(ConditionCodes::OF, error);
   assert(code == 0);
   assert(error == false);

   code = this->cc->getConditionCode(ConditionCodes::ZF, error);
   assert(code == 0);
   assert(error == false);

   code = this->cc->getConditionCode(ConditionCodes::SF, error);
   assert(code == 0);
   assert(error == false);

   //test the setting and clearing of each condition code
   doSetClearConditionCodeTests(ConditionCodes::OF);
   doSetClearConditionCodeTests(ConditionCodes::ZF);
   doSetClearConditionCodeTests(ConditionCodes::SF);

   int32_t ccNum;
   //test on some random ccNum values
   for (ccNum = -31; ccNum < 31; ccNum++)
       if (ccNum != ConditionCodes::OF && ccNum != ConditionCodes::ZF && 
           ccNum != ConditionCodes::SF)
          doErrorConditionCodeTests(ccNum);

   std::cout << "All ConditionCodes tests passed.\n"; 
}

/** 
 * doSetClearConditionCodeTests
 * Tests that the condition code identified by ccNum can be
 * set to true and can be set to false.
 *
 * @param - ccNum is either ConditionCodes::OF, ConditionCodes::SF, 
 *          or ConditionCodes::ZF
*/
void ConditionCodesTester::doSetClearConditionCodeTests(int32_t ccNum)
{
   bool error, code;
   //set the condition code to true
   this->cc->setConditionCode(true, ccNum, error);
   assert(error == false);
   code = this->cc->getConditionCode(ccNum, error);
   assert(error == false);
   assert(code == true);

   //set the condition code to false
   this->cc->setConditionCode(false, ccNum, error);
   assert(error == false);
   code = this->cc->getConditionCode(ccNum, error);
   assert(error == false);
   assert(code == false);
}

/**
 * doErrorConditionCodeTests
 * Tests the getConditionCode and setConditionCode methods in the 
 * ConditionCodes class by passing code numbers that are out of range.
 *
 * @param - ccNum is neither ConditionCodes::OF, ConditionCodes::SF, 
 *          nor ConditionCodes::ZF
 */
void ConditionCodesTester::doErrorConditionCodeTests(int32_t ccNum)
{
   bool error, code;
   //set the condition code to true
   this->cc->setConditionCode(true, ccNum, error);
   assert(error == true);
   code = this->cc->getConditionCode(ccNum, error);
   assert(error == true);
   assert(code == false);

   //set the condition code to false
   this->cc->setConditionCode(false, ccNum, error);
   assert(error == true);
   code = this->cc->getConditionCode(ccNum, error);
   assert(error == true);
   assert(code == false);
}


