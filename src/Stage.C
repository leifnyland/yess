#include "Memory.h"
#include "RegisterFile.h"
#include "ConditionCodes.h"
#include "Stage.h"

//initialization of the static variables
//in Stage.h
Memory * Stage::mem = NULL;
RegisterFile * Stage::rf = NULL;
ConditionCodes * Stage::cc = NULL;
uint64_t Stage::d_srcA = RegisterFile::RNONE; 
uint64_t Stage::d_srcB = RegisterFile::RNONE; 
uint64_t Stage::e_Cnd = 0;
uint64_t Stage::e_valE = 0;
uint64_t Stage::e_dstE = RegisterFile::RNONE; 
uint64_t Stage::m_stat = 0;
uint64_t Stage::m_valM = 0;

//Stage constructor
//initialize the static data members: mem, cc, rf
Stage::Stage()
{
   mem = Memory::getInstance();
   cc = ConditionCodes::getInstance();
   rf = RegisterFile::getInstance();
}
