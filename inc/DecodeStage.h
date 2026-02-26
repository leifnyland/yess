#include "PipeRegArray.h"
#include "Stage.h"

#ifndef DECODESTAGE_H
#define DECODESTAGE_H
class DecodeStage: public Stage
{
    private:
		bool E_bubble;
		//Built in methods here
		uint64_t getd_srcA(uint64_t icode, uint64_t D_rA, uint64_t rsp, uint64_t rnone);
		uint64_t getd_srcB(uint64_t icode, uint64_t D_rB, uint64_t rsp, uint64_t rnone);
		uint64_t getd_dstE(uint64_t icode, uint64_t D_rB, uint64_t rsp, uint64_t rnone);
		uint64_t getd_dstM(uint64_t icode, uint64_t D_rA, uint64_t rnone);
		uint64_t getd_valA(uint64_t icode, uint64_t srcA, uint64_t d_rvalA, uint64_t valP, 
							uint64_t M_dstM, uint64_t M_dstE, uint64_t M_valE, uint64_t W_dstM,
							uint64_t W_valM, uint64_t W_dstE, uint64_t W_valE);
		uint64_t getd_valB(uint64_t srcB, uint64_t d_rvalB, uint64_t M_dstM, uint64_t M_dstE,
							uint64_t M_valE, uint64_t W_dstM, uint64_t W_valM, 
							uint64_t W_dstE, uint64_t W_valE);
		void setEInput(PipeReg * dreg, uint64_t stat, uint64_t icode,
							uint64_t ifun, uint64_t valC, uint64_t valA,
							uint64_t valB, uint64_t dstE, uint64_t dstM,
							uint64_t srcA, uint64_t srcB);
		bool e_bubble(uint64_t E_icode, uint64_t E_dstM, bool e_Cnd);
		void calculateControlSignals(uint64_t E_icode, uint64_t E_dstM);
	  

	public:
		//These are the only methods called outside of the class
		bool doClockLow(PipeRegArray * pipeRegs);
		void doClockHigh(PipeRegArray * pipeRegs);
};
#endif
