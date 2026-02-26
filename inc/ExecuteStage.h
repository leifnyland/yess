#include "PipeRegArray.h"
#include "Stage.h"

#ifndef EXECUTESTAGE_H
#define EXECUTESTAGE_H
class ExecuteStage: public Stage
{
   private:
	  bool M_Bubble;
	  void setMInput(PipeReg * dreg, uint64_t stat, uint64_t icode,
                           uint64_t cnd, uint64_t valE, uint64_t valA,
                           uint64_t dstE, uint64_t dstM);
	  uint64_t aluA(PipeReg * ereg);
	  uint64_t aluB(PipeReg * ereg);
	  uint64_t alufun(PipeReg * ereg);
	  bool set_cc(PipeReg * ereg, PipeReg * wreg);
	  uint64_t e_dstE(PipeReg * ereg, uint64_t dstE, uint64_t cnd);
	  void ALU(PipeReg * ereg);
	  void CC(PipeReg * ereg, PipeReg * wreg);
	  uint64_t e_cnd(uint64_t icode, uint64_t ifun);
	  bool M_bubble(uint64_t W_stat, uint64_t m_stat);
	  void calculateControlSignals(uint64_t W_stat, uint64_t m_stat);
   public:
      //These are the only methods called outside of the class
      bool doClockLow(PipeRegArray * pipeRegs);
      void doClockHigh(PipeRegArray * pipeRegs);
};
#endif
