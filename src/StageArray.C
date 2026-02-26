#include "StageArray.h"
#include "Stage.h"
#include "FetchStage.h"
#include "DecodeStage.h"
#include "ExecuteStage.h"
#include "MemoryStage.h"
#include "WritebackStage.h"

/*
 * StageArray
 * construct and initialize the array of stages
 */
StageArray::StageArray()
{
   stages = new Stage * [NUMSTAGES];
   stages[FSTAGE] = new FetchStage();
   stages[DSTAGE] = new DecodeStage();
   stages[ESTAGE] = new ExecuteStage();
   stages[MSTAGE] = new MemoryStage();
   stages[WSTAGE] = new WritebackStage();
}

/*
 * return a pointer to the FetchStage object 
 */
Stage * StageArray::getFetchStage()
{
    return stages[FSTAGE];
}

/*
 * return a pointer to the DecodeStage object 
 */
Stage * StageArray::getDecodeStage()
{
    return stages[DSTAGE];
}

/*
 * return a pointer to the ExecuteStage object 
 */
Stage * StageArray::getExecuteStage()
{
    return stages[ESTAGE];
}

/*
 * return a pointer to the MemoryStage object 
 */
Stage * StageArray::getMemoryStage()
{
    return stages[MSTAGE];
}

/*
 * return a pointer to the WritebackStage object 
 */
Stage * StageArray::getWritebackStage()
{
    return stages[WSTAGE];
}
