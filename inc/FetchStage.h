#include "PipeRegArray.h"
#include "PipeReg.h"
#include "Stage.h"

#ifndef FETCHSTAGE_H
#define FETCHSTAGE_H


class FetchStage: public Stage
{
   private:
	  bool F_stall;
	  bool D_stall;
	  bool D_bubble;
      //provide declarations for new methods
      void setDInput(PipeReg * dreg, uint64_t stat, uint64_t icode, uint64_t ifun, 
                     uint64_t rA, uint64_t rB,
                     uint64_t valC, uint64_t valP);
	  bool instr_valid(uint64_t f_icode);
	  uint64_t f_stat(bool mem_error, bool instr_valid, uint64_t f_icode);
	  uint64_t selectPC(uint64_t M_icode, uint64_t m_Cnd, uint64_t M_valA, uint64_t W_icode, uint64_t W_valM, uint64_t predPC);
	  bool needRegIDs (uint64_t f_icode);
	  bool needValC (uint64_t f_icode);
	  uint64_t predictPC(uint64_t f_icode, uint64_t f_valC, uint64_t f_valP);
	  uint64_t PCincrement(uint64_t f_pc, bool needsRegIds, bool needsValC);
	  void getRegIDs(uint64_t f_pc, uint64_t regByte, uint64_t &rA, uint64_t &rB);
	  uint64_t buildValC(Memory * mem, uint64_t f_pc, bool mem_error, uint64_t f_icode);
	  bool f_stall(uint64_t D_icode, uint64_t E_icode, uint64_t M_icode, uint64_t E_dstM);
	  bool d_stall(uint64_t E_icode, uint64_t E_dstM);
	  bool d_bubble(uint64_t D_icode, uint64_t E_icode, uint64_t M_icode, uint64_t E_dstM, bool e_Cnd);
	  void calculateControlSignals(uint64_t D_icode, uint64_t E_icode, uint64_t M_icode, uint64_t E_dstM);

   public:
      //These are the only methods called outside of the class
      bool doClockLow(PipeRegArray * pipeRegs);
      void doClockHigh(PipeRegArray * pipeRegs);
};
#endif
