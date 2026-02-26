#include <cstdint>
#include "PipeRegField.h"

/* 
 * PipeRegField constructor
 *
 * initialize the PipeRegField
 * default value is 0 if no parameter is given
*/
PipeRegField::PipeRegField(int32_t state)
{
   input = 0;
   this->state = state;
}

/*
 * setInput
 *
 * @param value to be stored in input data field
 *
*/
void PipeRegField::setInput(uint64_t input)
{
   this->input = input;
}

/*
 * getOutput
 *
 * @return the state data field
*/
uint64_t PipeRegField::getOutput()
{
   return state;
}

/*
 * normal
 *
 * simulates the normal control signal applied
 * to a pipelined register by setting state
 * to the input
*/
void PipeRegField::normal()
{
   state = input;
} 

/*
 * setState
 *
 * Changes the state of the pipe register field
 * This can be used for bubbling the pipe register
 *
 * @param value to set state to
*/  
void PipeRegField::setState(int32_t state)
{
   this->state = state;
}

