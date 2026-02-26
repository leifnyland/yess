#include<fstream>
#include <iostream>
#include <cstdint>
#include "Memory.h"
#include "String.h"

#ifndef LOADER_H
#define LOADER_H
class Loader
{
   private:
      //error numbers for loader errors
      //see printErrMsg
      static const int32_t usage = 0;      //bad command line args
      static const int32_t badfile = 1;    //no .yo suffix
      static const int32_t openerr = 2;    //file doesn't open
      static const int32_t baddata = 3;    //bad data record
      static const int32_t badcomment = 4; //bad comment record
      static const int32_t numerrs = 5;    //number error types

      //constants for accessing byes from
      //input line
      static const int32_t addrbegin = 2;  //begin address index
      static const int32_t addrend = 4;    //end address index
      static const int32_t databegin = 7;  //begin data index
      static const int32_t comment = 28;   //index of |
      static const int32_t maxbytes = 10;  //max bytes in data record

      int32_t lastAddress;  //last address stored to in memory
      Memory * mem;         //pointer to memory instance
      std::ifstream inf;    //file handle
      String * inputFile;   //pointer to String object containing file name

      bool openFile();      //check file name for correctness and open it
      bool printErrMsg(int32_t, int32_t, String *);  //print error message

      //checks length of data
	  int lengthOfData(String lineIn); 
	  //gets line address
	  uint32_t getAddress(String lineIn); 
	  //checks a line for propper formatting
	  bool checkLine(String lineIn, int lineNumber);
	  


      
   public:
      //methods called outside of class
      Loader(int argc, char * argv[], Memory * mem);
      bool load();
};
#endif
