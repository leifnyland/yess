/* 
 * Driver for the yess simulator
 * Usage: yess <file>.yo [-D]
 *
 * <file>.yo contains assembled y86-64 code.
 * If the -D option is provided then debug is set to 1.
 * The -D option can be used to turn on and turn off debugging print
 * statements.
*/
#include "Memory.h"
#include "Loader.h"
#include "CPU.h"

int main(int argc, char * argv[])
{

   Memory * mem = Memory::getInstance();
   Loader loadObj(argc, argv, mem);
   if (!loadObj.load())
   {
      if (mem != NULL) mem->dump();
      return 0;
   }
   CPU cpu;
   cpu.run(); 
   return 0;
}

