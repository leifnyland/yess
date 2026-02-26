#include <iostream>
#include <fstream>
#include <cstdint>
#include "Memory.h"
#include "String.h"
#include "Loader.h"

/* 
 * Loader
 * Initializes the private data members
 */
Loader::Loader(int argc, char * argv[], Memory * mem)
{
   //this method is COMPLETE
   this->lastAddress = -1;   //keep track of last mem byte written to for error checking
   this->mem = mem;          //memory instance
   this->inputFile = NULL;   
   if (argc > 1) inputFile = new String(argv[1]);  //input file name
}

/*
 * printErrMsg
 * Prints an error message and returns false (load failed)
 * If the line number is not -1, it also prints the line where error occurred
 *
 * which - indicates error number
 * lineNumber - number of line in input file on which error occurred (if applicable)
 * line - line on which error occurred (if applicable)
 */
bool Loader::printErrMsg(int32_t which, int32_t lineNumber, String * line)
{
   //this method is COMPLETE
   static char * errMsg[Loader::numerrs] = 
   {
      (char *) "Usage: yess <file.yo>\n",                       //Loader::usage
      (char *) "Input file name must end with .yo extension\n", //Loader::badfile
      (char *) "File open failed\n",                            //Loader::openerr
      (char *) "Badly formed data record\n",                    //Loader::baddata
      (char *) "Badly formed comment record\n",                 //Loader::badcomment
   };   
   if (which >= Loader::numerrs)
   {
      std::cout << "Unknown error: " << which << "\n";
   } else
   {
      std::cout << errMsg[which]; 
      if (lineNumber != -1 && line != NULL)
      {
         std::cout << "Error on line " << std::dec << lineNumber
                   << ": " << line->get_stdstr() << std::endl;
      }
   } 
   return false; //load fails
}

/*
 * openFile
 * The name of the file is in the data member openFile (could be NULL if
 * no command line argument provided)
 * Checks to see if the file name is well-formed and can be opened
 * If there is an error, it prints an error message and returns false
 * by calling printErrMsg
 * Otherwise, the file is opened and the function returns true
 *
 * modifies inf data member (file handle) if file is opened
 */
bool Loader::openFile()
{
	String * ptr = 0;
   //If the user didn't supply a command line argument (inputFile is NULL)
   //then print the Loader::usage error message and return false
   //(Note: Loader::usage is a static const defined in Loader.h)

	if (inputFile == NULL) {
		printErrMsg(Loader::usage, 4, ptr);
		return false;
	}

   //If the filename is badly formed (needs to be at least 4 characters
   //long and end with .yo) then print the Loader::badfile error message 
   //and return false

   String input = String(*(inputFile));
   bool e = false;
   int32_t l = input.get_length();
   if (l < 4 || !(input.isSubString(".yo",input.get_length()-3,e))) {
	 printErrMsg(Loader::badfile,4,ptr);
	 return false;
   }
   //Open the file using an std::ifstream open
   //If the file can't be opened then print the Loader::openerr message 
   //and return false
   Loader::inf.open(input.get_stdstr(), std::ifstream::in);
   
   if (!(Loader::inf.is_open())) {
	printErrMsg(Loader::openerr,4,ptr);
	return false;
   }

   return true;  //file name is good and file open succeeded
}





/*
 * load 
 * Opens the .yo file by calling openFile.
 * Reads the lines in the file line by line and
 * loads the data bytes in data records into the Memory.
 * If a line has an error in it, then NONE of the line will be
 * loaded into memory and the load function will return false.
 *
 * Returns true if load succeeded (no errors in the input) 
 * and false otherwise.
*/   
bool Loader::load()
{
   
   if (!openFile()) return false;
   int32_t currentAddress = 0;
   
   std::string line;
   int lineNumber = 1;  //needed if an error is found
   while (getline(inf, line))
   {
      //create a String to contain the std::string
      //Now, all accesses to the input line MUST be via your
      //String class methods
      String inputLine(line);

	  int dataLength = lengthOfData(inputLine);
	  int dataAddress = getAddress(inputLine);
	  bool checkedLine = checkLine(inputLine, lineNumber);
	  //checkedLine is false if there are no errors
	  if (!checkedLine) {
		if ((currentAddress > dataAddress && dataAddress != 0) || ((dataAddress > 999) && (dataLength > 0))) {
			Loader::printErrMsg((int32_t)3, (int32_t)lineNumber, &inputLine);
			return false;
		}
		currentAddress = dataLength / 2 + dataAddress;
	  } else {
		return false;
	  }


	  bool bDummy = false;
	  if (!checkedLine) { //if it might contain Data

		uint8_t abyte = inputLine.convert2Hex(7,2,bDummy); 			//gets Data
		int32_t byteLoc = inputLine.convert2Hex(2,3,bDummy); 		//gets Address
		bDummy = bDummy || !inputLine.isHex(7,2,bDummy);
		int counter = 0;
		while (!bDummy  && counter < 30) {
			//checks for another Byte
			bDummy = bDummy || !inputLine.isHex(9 + 2 * counter,2,bDummy);
			//stores Byte
			mem->putByte(abyte, (byteLoc + counter),bDummy); 			
			//gets new Byte
			abyte = inputLine.convert2Hex(9 + 2 * counter,2,bDummy);
			counter++;
		}
	  }

      //increment the line number for the next iteration
      lineNumber++;
   }

   return true;  //load succeeded

}

//Add helper methods definitions here and the declarations to Loader.h
//In your code, be sure to use the static const variables defined in 
//Loader.h to grab specific fields from the input line.

/*
* Returns the length of the data stored, if it is in the correct format
*/
int Loader::lengthOfData(String lineIn) {
	bool bDummy = false;
	int infoLength = 0;
	for (int i = 0; i <= 20; i++) {
		if (!(lineIn.isHex(i + 7,1,bDummy)) || bDummy) {
			infoLength = i;
			break;
		}
	}
	return infoLength;
}

/*
*  Returns the address listed on the line, if it is in the correct format
*/
uint32_t Loader::getAddress(String lineIn) {
	bool bDummy2 = false;
	return lineIn.convert2Hex(2, 3, bDummy2);
}


/*
*  Checks a line for format errors and returns false if the line is good.
*  Prints both comment and data errors
*/
bool Loader::checkLine(String lineIn, int lineNumber) {

	//return values
	bool error;    //Badly formed data error
	bool comError; //Badly formed comment error
	//error flag for all the String methods
	bool errFlag;
	
	// line contains an address - data not required
	if (lineIn.isChar('0', 0, errFlag)) { 
		//checks the first character
		error = !lineIn.isChar('0', 0, errFlag);
		//checks for a missing x in address
		comError = !lineIn.isChar('x', 1, errFlag);
		//checks to see if memory address is hex
		comError = comError || !lineIn.isHex(2, 3, errFlag);
		//checks for the colon and space after the memory location
		error = error || !lineIn.isSubString(": ", 5, errFlag);
		//checks if all the characters that are after hex data are spaces
		error = error || !lineIn.isRepeatingChar(' ', 7 + Loader::lengthOfData(lineIn), 21 - Loader::lengthOfData(lineIn), errFlag);
		//checks to make sure the instruction is an even length
		error = error || Loader::lengthOfData(lineIn) % 2 != 0;
		//looks for a "|" at the correct location
		error = error || !lineIn.isChar('|', 28, errFlag);
	}
	//when the first char is not 0 or a space
	else if (!lineIn.isChar(' ', 0, errFlag)) {
		comError = true; //can't be any other char
	}
	// line does not have any machine code
	else {
		error=false;
		//checks to make sure that the instruction portion of the file is empty and that there is still a "|" at the correct location
		comError = !lineIn.isRepeatingChar(' ', 0, 28, errFlag);
		//throws comError if line is empty
		comError = comError || !lineIn.isChar('|', 28, errFlag);
	}
	//Returns a badly formed data error
	if (error) {
		Loader::printErrMsg((int32_t)3, (int32_t)lineNumber, &lineIn);
		return true;
	}
	//Returns a badly formed comment error
	if (comError) {
		Loader::printErrMsg((int32_t)4, (int32_t)lineNumber, &lineIn);
		return true;
	}
	return false;
}