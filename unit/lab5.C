#include <iostream>
#include <fstream>
#include <cstdint>
#include <string>
#include <string.h>
#include <assert.h>
#include "String.h"

//perform unit tests on the String class
int main(int argc, char * argv[])
{

   // test the constructor and the getters
   String * str1 = new String("hello there");
   assert(strcmp("hello there", str1->get_cstr()) == 0);
   assert(strlen("hello there") == str1->get_length());
  
   std::string stdstr2("hello world");
   String * str2 = new String(stdstr2);
   assert(stdstr2.compare(str2->get_stdstr()) == 0);
   assert(stdstr2.length() == (uint32_t) str2->get_length());

   //If the line below is uncommented, it should cause the String
   //constructor assert to fail.
   //String * strtmp = new String("");

   //test the isRepeatingChar method
   String str3("aaaaaphrase with    spaces ");
   bool error;
   assert(str3.isRepeatingChar('a', 1, 4, error) == true);
   assert(error == false);
   assert(str3.isRepeatingChar(' ', 0, 5, error) == false);
   assert(error == false);
   assert(str3.isRepeatingChar('a', 1, 5, error) == false);
   assert(error == false);
   assert(str3.isRepeatingChar(' ', 11, 1, error) == true);
   assert(error == false);
   assert(str3.isRepeatingChar(' ', 4, 8, error) == false);
   assert(error == false);
   assert(str3.isRepeatingChar(' ', 11, 2, error) == false);
   assert(error == false);
   assert(str3.isRepeatingChar(' ', 17, -3, error) == false);
   assert(error == true);
   assert(str3.isRepeatingChar(' ', 26, 1, error) == true);
   assert(error == false);
   assert(str3.isRepeatingChar(' ', 26, 2, error) == false);
   assert(error == true);
   assert(str3.isRepeatingChar(' ', -2, -1, error) == false);
   assert(error == true);

   String str4("0x2aA: 123e5678");
   //test the isHex method
   assert(str4.isHex(2, 3, error) == true);
   assert(error == false);
   assert(str4.isHex(7, 2, error) == true);
   assert(error == false);
   assert(str4.isHex(9, 2, error) == true);
   assert(error == false);
   assert(str4.isHex(11, 2, error) == true);
   assert(error == false);
   assert(str4.isHex(13, 2, error) == true);
   assert(error == false);
   assert(str4.isHex(14, 2, error) == false);
   assert(error == true);
   assert(str4.isHex(16, 2, error) == false);
   assert(error == true);
   assert(str4.isHex(8, -1, error) == false);
   assert(error == true);
   assert(str4.isHex(-1, 7, error) == false);
   assert(error == true);
   assert(str4.isHex(-2, -1, error) == false);
   assert(error == true);
   
   //test the convert2Hex method
   assert(str4.convert2Hex(2, 3, error) == 0x2aA);
   assert(error == false);
   assert(str4.convert2Hex(7, 2, error) == 0x12);
   assert(error == false);
   assert(str4.convert2Hex(9, 2, error) == 0x3e);
   assert(error == false);
   assert(str4.convert2Hex(11, 2, error) == 0x56);
   assert(error == false);
   assert(str4.convert2Hex(13, 2, error) == 0x78);
   assert(error == false);
   assert(str4.convert2Hex(14, 2, error) == 0);
   assert(error == true);
   assert(str4.convert2Hex(16, 2, error) == 0);
   assert(error == true);
   assert(str4.convert2Hex(8, -1, error) == 0);
   assert(error == true);
   assert(str4.convert2Hex(-1, 7, error) == 0);
   assert(error == true);
   assert(str4.convert2Hex(-2, -1, error) == 0);
   assert(error == true);

   String str5("0xabc: def01234");
   //test the isHex method
   assert(str5.isHex(2, 3, error) == true);
   assert(error == false);
   assert(str5.isHex(7, 2, error) == true);
   assert(error == false);
   assert(str5.isHex(9, 2, error) == true);
   assert(error == false);
   assert(str5.isHex(11, 2, error) == true);
   assert(error == false);
   assert(str5.isHex(13, 2, error) == true);
   assert(error == false);

   //test the isChar method
   String str8("0xabc: abc123");
   assert(str8.isChar('0', 0, error) == true);
   assert(error == false);
   assert(str8.isChar('x', 1, error) == true);
   assert(error == false);
   assert(str8.isChar(':', 5, error) == true);
   assert(error == false);
   assert(str8.isChar(';', 5, error) == false);
   assert(error == false);
   assert(str8.isChar(';', -1, error) == false);
   assert(error == true);
   assert(str8.isChar(';', 13, error) == false);
   assert(error == true);

   //test the convert2Hex method
   assert(str5.convert2Hex(2, 3, error) == 0xabc);
   assert(error == false);
   assert(str5.convert2Hex(7, 2, error) == 0xde);
   assert(error == false);
   assert(str5.convert2Hex(9, 2, error) == 0xf0);
   assert(error == false);
   assert(str5.convert2Hex(11, 2, error) == 0x12);
   assert(error == false);
   assert(str5.convert2Hex(13, 2, error) == 0x34);
   assert(error == false);

   String str6("0xABC: DEF56789");
   //test the isHex method
   assert(str6.isHex(2, 3, error) == true);
   assert(error == false);
   assert(str6.isHex(7, 2, error) == true);
   assert(error == false);
   assert(str6.isHex(9, 2, error) == true);
   assert(error == false);
   assert(str6.isHex(11, 2, error) == true);
   assert(error == false);
   assert(str6.isHex(13, 2, error) == true);
   assert(error == false);

   //test the convert2Hex method
   assert(str6.convert2Hex(2, 3, error) == 0xabc);
   assert(error == false);
   assert(str6.convert2Hex(7, 2, error) == 0xde);
   assert(error == false);
   assert(str6.convert2Hex(9, 2, error) == 0xf5);
   assert(error == false);
   assert(str6.convert2Hex(11, 2, error) == 0x67);
   assert(error == false);
   assert(str6.convert2Hex(13, 2, error) == 0x89);
   assert(error == false);

   String str7("0xAgC: DEi56789");
   //test the isHex method
   assert(str7.isHex(2, 3, error) == false);
   assert(error == false);
   assert(str7.isHex(9, 2, error) == false);
   assert(error == false);
   
   //test the convert2Hex method
   assert(str7.convert2Hex(2, 3, error) == 0);
   assert(error == true);
   assert(str7.convert2Hex(9, 2, error) == 0);
   assert(error == true);

   //test the isSubString method passing it a char *
   assert(str8.isSubString("0x", 0, error) == true);
   assert(error == false);
   assert(str8.isSubString("abc", 2, error) == true);
   assert(error == false);
   assert(str8.isSubString("abc123", 7, error) == true);
   assert(error == false);
   assert(str8.isSubString("abc123", 5, error) == false);
   assert(error == false);
   assert(str8.isSubString("abc123", 8, error) == false);
   assert(error == true);
   assert(str8.isSubString("abc123", -1, error) == false);
   assert(error == true);

   //test the isSubString method passing it a std::string
   assert(str8.isSubString(std::string("0x"), 0, error) == true);
   assert(error == false);
   assert(str8.isSubString(std::string("abc"), 2, error) == true);
   assert(error == false);
   assert(str8.isSubString(std::string("abc123"), 7, error) == true);
   assert(error == false);
   assert(str8.isSubString(std::string("abc123"), 5, error) == false);
   assert(error == false);
   assert(str8.isSubString(std::string("abc123"), 8, error) == false);
   assert(error == true);
   assert(str8.isSubString(std::string("abc123"), -1, error) == false);
   assert(error == true);

   //test the isHex method

   std::cout << "Congratuations!  All tests pass!\n"; 
}
