#include <cstdint>
#include <string>
#include "PipeReg.h"
#include "PipeRegArray.h"
#include "F.h"
#include "D.h"
#include "E.h"
#include "M.h"
#include "W.h"

/*
 * created and initialize the PipeReg array
 */
PipeRegArray::PipeRegArray()
{
   pregs = new PipeReg * [NUMPIPEREGS];
   pregs[FREG] = new F();
   pregs[DREG] = new D();
   pregs[EREG] = new E();
   pregs[MREG] = new M();
   pregs[WREG] = new W();
}

/*
 * return a pointer to the F PipeReg object
 */
PipeReg * PipeRegArray::getFetchReg()
{
    return pregs[FREG];
}

/*
 * return a pointer to the D PipeReg object
 */
PipeReg * PipeRegArray::getDecodeReg()
{
    return pregs[DREG];
}

/*
 * return a pointer to the E PipeReg object
 */
PipeReg * PipeRegArray::getExecuteReg()
{
    return pregs[EREG];
}

/*
 * return a pointer to the M PipeReg object
 */
PipeReg * PipeRegArray::getMemoryReg()
{
    return pregs[MREG];
}

/*
 * return a pointer to the W PipeReg object
 */
PipeReg * PipeRegArray::getWritebackReg()
{
    return pregs[WREG];
}
