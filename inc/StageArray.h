#include "Stage.h"

#ifndef STAGEARRAY_H
#define STAGEARRAY_H

//these #defines can be used as indices in an array of stages
#define FSTAGE 0
#define DSTAGE 1
#define ESTAGE 2
#define MSTAGE 3
#define WSTAGE 4
//five stages: FetchStage, DecodeStage, ExecuteStage,
//             MemoryStage, WritebackStage
#define NUMSTAGES 5
class StageArray
{
   private:
      Stage ** stages;
   public:
      StageArray();
      Stage * getFetchStage();
      Stage * getDecodeStage();
      Stage * getExecuteStage();
      Stage * getMemoryStage();
      Stage * getWritebackStage();
};
#endif
