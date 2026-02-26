#include <cstdint>
#include "Tools.h"
#include <cstdlib>
#include <string>

/*
 * Hints/Notes:
 * 1) Pay attention to what the comments say. 
 * 2) Sometimes a function returns the source input if input validation
 *    fails and sometimes it returns 0.
 * 3) Pay attention to the "rules".
 * 4) You won't get credit for a method if it passes the tests but
 *    breaks the "rules".
 * 5) if the rule is no loops then you cannot use while, do-while, for
 * 6) if the rule is no conditionals then you cannot use if-else, ternary statements
 *    but you can use conditional expressions: eq = (a == b);
 * 7) If you shift a variable by an amount that is equal to or
 *    larger than its size, then the shift doesn't happen. For example, 
 *    if n is a 64 bit unsigned int then n >> 64 is n (not 0).
 * 8) If you need a 64 bit constant, you need to follow the constant
 *    by either ul (unsigned long), l (long), ull (unsigned long long),
 *    or ll (long long). For example, 3ul, 3l, 3ull, 3ll are all eight bytes.
 *    By default, 3 without the suffix is a 32 bit int.
 * 9) >> (right shift) shifts off low order bits
 * 10) << (left shift) shifts off high order bits
 * 11) right shift of a signed variable maintains the sign bit; right shift of an
 *     unsigned variable brings in 0
 *
 */

/** 
 * builds a 64-bit long out of an array of 8 bytes
 *
 * @param array of 8 bytes
 * @return uint64_t where the low order byte is bytes[0] and
 *         the high order byte is bytes[7]
 *
 * RULES: 
 *   1) no more than ten lines of code
*/
uint64_t Tools::buildLong(uint8_t bytes[LONGSIZE])
{
  uint64_t x = 0L;
  for (int i = 7; i >= 0; i--) {
    x = x << 8;
    x += bytes[i];
  }
  return x;
}

/** 
 * accepts as input an uint64_t and returns the designated byte
 * within the uint64_t; returns 0 if the indicated byte number
 * is out of range 
 *
 * for example, getByte(0x1122334455667788, 7) returns 0x11
 *              getByte(0x1122334455667788, 1) returns 0x77
 *              getByte(0x1122334455667788, 8) returns 0
 *
 * @param uint64_t source that is the source data
 * @param int32_t byteNum that indicates the byte to return (0 through 7)
 *                byte 0 is the low order byte
 * @return 0 if byteNum is out of range
 *         byte 0, 1, .., or 7 of source if byteNum is within range
 *
 * RULES:
 * 1) you can use an if to handle error checking on input
 * 2) no loops or conditionals or switch other than 1)
*/
uint64_t Tools::getByte(uint64_t source, int32_t byteNum)
{
  if (byteNum < 0 || byteNum > 7) {
    return 0x00;
  }
  uint64_t x = source >> (byteNum * 8);
  //x = x >> (byteNum * 8);
  
  x = x << 56;
  x = x >> 56;
  return x;
}

/**
 * accepts as input an uint64_t and returns the bits low through 
 * high of the uint64_t. bit 0 is the low order bit and bit 63
 * is the high order bit. returns 0 if the low or high bit numbers 
 * are out of range
 *
 * for example, getBits(0x8877665544332211, 0, 7) returns 0x11
 *              getBits(0x8877665544332211, 4, 11) returns 0x21
 *              getBits(0x8877665544332211, 0, 63) returns 0x8877665544332211
 *              getBits(0x8877665544332211, 0, 64) returns 0
 *
 * @param uint64_t source that holds the bits to be grabbed and 
 *        returned
 * @param int32_t low that is the bit number of the lowest numbered
 *        bit to be returned
 * @param int32_t high that is the bit number of the highest numbered
 *        bit to be returned
 * @return an uint64_t that holds a subset of the source bits
 *         that is returned in the low order bits; 0 if low or high 
 *         is out of range
 *
 * RULES:
 * 1) you can use an if to handle error checking on input
 * 2) no loops or conditionals or switch other than 1)
 */
uint64_t Tools::getBits(uint64_t source, int32_t low, int32_t high)
{
  if (low < 0 || low > 63 || high < 0 || high > 63 || low > high) {
    return 0x00;
  }
  uint64_t x = source >> low;
  x = x << (63 - high + low);
  x = x >> (63 - high + low);
  return x;
}

/**
 * sets the bits of source in the range specified by the low and high
 * parameters to 1 and returns that value. returns source if the low or high
 * bit numbers are out of range
 *
 * for example, setBits(0x1122334455667788, 0, 7) returns 0x11223344556677ff
 *              setBits(0x1122334455667788, 8, 0xf) returns 0x112233445566ff88
 *              setBits(0x1122334455667788, 8, 64) returns 0x1122334455667788
 *                      note: 64 is out of range
 *
 * @param uint64_t source 
 * @param int32_t low that is the bit number of the lowest numbered
 *        bit to be set to 1
 * @param int32_t high that is the bit number of the highest numbered
 *        bit to be set to 1
 * @return an uint64_t that holds the modified source
 *
 * RULES:
 * 1) you can use an if to handle error checking on input
 * 2) no loops or conditionals or switch other than 1)
 * 3) you can use other functions you have written, for example, getBits
 */
uint64_t Tools::setBits(uint64_t source, int32_t low, int32_t high)
{
  if (low < 0 || low > 63 || high < 0 || high > 63 || low > high) {
    return source;
  }
  uint64_t y = 0xffffffffffffffff;
  y = y << (63 - high + low);
  y = y >> (63 - high + low);
  y = y << low;
  return source | y;
}

/**
 * sets the bits of source in the range low to high to 0 (clears them) and
 * returns that value. returns source if the low or high
 * bit numbers are out of range
 *
 * for example, clearBits(0x1122334455667788, 0, 7) returns 0x1122334455667700
 *              clearBits(0x1122334455667788, 8, f) returns 0x1122334455660088
 *
 * @param uint64_t source 
 * @param int32_t low that is the bit number of the lowest numbered
 *        bit to be set to 0
 * @param int32_t high that is the bit number of the highest numbered
 *        bit to be set to 0
 * @return an uint64_t that holds the modified source
 *
 * RULES:
 * 1) you can use an if to handle error checking on input
 * 2) no loops or conditionals or switch other than 1)
 * 3) you can use other functions you have written, for example, getBits
 */
uint64_t Tools::clearBits(uint64_t source, int32_t low, int32_t high)
{
  if (low < 0 || low > 63 || high < 0 || high > 63 || low > high) {
    return source;
  }
  uint64_t y = 0xffffffffffffffff;
  y = y << (63 - high + low);
  y = y >> (63 - high + low);
  y = y << low;
  y = ~y;
  return source & y;
}

/**
 * flips the bits of source in the range specified by the low and high
 * parameters and returns that value. returns source if the low or high
 * bit numbers are out of range
 *  
 * for example, flipBits(0x1122334455667788, 0, 7) returns 0x1122334455667777
 *              flipBits(0x1122334455667788, 8, 0xf) returns 0x1122334455668888
 *              flipBits(0x1122334455667788, 8, 64) returns 0x1122334455667788
 *                                                  note: 64 is out of range
 *  
 * @param uint64_t source 
 * @param int32_t low that is the bit number of the lowest numbered
 *                    bit to be set to 1
 * @param int32_t high that is the bit number of the highest numbered
 *                     bit to be set to 1
 * @return an uint64_t that holds the modified source
 * 
 * RULES:
 * 1) you can use an if to handle error checking on input
 * 2) no loops or conditionals or switch other than 1)
 * 3) you can use other functions you have written, for example, getBits
 */
uint64_t Tools::flipBits(uint64_t source, int32_t low, int32_t high)
{
  if (low < 0 || low > 63 || high < 0 || high > 63 || low > high) {
    return source;
  }
  uint64_t x = (~source) >> low;
  x = x << (63 - high + low);  //gets the flipped numbers
  x = x >> (63 - high);
  return x | clearBits(source,low,high); //combines it with source
}

/**
 * copies length bits from the source to a destination and returns the
 * modified destination. If low bit number of the source or 
 * dest is out of range or the calculated source or dest high bit 
 * number is out of range, then the unmodified destination is returned.
 *
 * for example,
 *   copyBits(0x1122334455667788, 0x8877665544332211, 0, 0, 8) 
 *           returns 0x8877665544332288
 *   copyBits(0x1122334455667788, 0x8877665544332211, 0, 8, 8) 
 *           returns 0x8877665544338811
 *
 * @param uint64_t source 
 * @param uint64_t dest 
 * @param int32_t srclow that is the bit number of the lowest numbered
 *        bit of the source to be copied
 * @param int32_t destlow that is the bit number of the lowest numbered
 *        bit of the destination to be modified
 * @param int32_t length that is the number of bits to be copied
 * @return uint64_t that is the modifed dest
 *
 * RULES:
 * 1) you can use an if to handle error checking on input
 * 2) no loops or conditionals or switch other than 1)
 * 3) you can use other functions you have written, for example, getBits
 */
uint64_t Tools::copyBits(uint64_t source, uint64_t dest, 
                         int32_t srclow, int32_t dstlow, int32_t length)
{
  if (length <= 0 || length + dstlow  > 64 || length + srclow  > 64 || srclow < 0 || length < 0) {
    return dest;
  }
  uint64_t x = source >> srclow;
  x = x << (64 - length);  //gets the source bits
  x = x >> (64 - length - dstlow);
  return x | clearBits(dest,dstlow,dstlow + length - 1); //combines it with an emptied destination
}

/**
 * sets the byte of source identfied by the byte number to 0xff and
 * returns that value. if the byte number is out of range then source
 * is returned unchanged.
 *
 * for example, setByte(0x1122334455667788, 0) returns 0x11223344556677ff
 *              setByte(0x1122334455667788, 1) returns 0x112233445566ff88
 *              setByte(0x1122334455667788, 8) returns 0x1122334455667788
 *
 * @param uint64_t source 
 * @param int32_t byteNum that indicates the number of the byte to be
 *        set to 0xff; the low order byte is byte number 0
 * @return uint64_t that is source with byte byteNum set to 0xff
 *
 * RULES:
 * 1) you can use an if to handle error checking on input
 * 2) no loops or conditionals or switch other than 1)
 * 3) you can use other functions you have written, for example, getBits
 */
uint64_t Tools::setByte(uint64_t source, int32_t byteNum)
{
  if (byteNum >= 8 || byteNum < 0) {
    return source;
  }
  uint64_t x = 0xff;
  x = x << (byteNum * 8);
  return x | source;
}

/**
 * assumes source contains a 64 bit two's complement value and
 * returns the sign (1 or 0)
 *
 * for example, sign(0xffffffffffffffff) returns 1
 *              sign(0x0000000000000000) returns 0
 *              sign(0x8000000000000000) returns 1
 *
 * @param uint64_t source
 * @return 1 if source is negative when treated as a two's complement 
 *         value and 0 otherwise
 *
 * RULES:
 * 1) no loops or conditionals or switch
 * 2) you can use other functions you have written, for example, getBits
 */
uint64_t Tools::sign(uint64_t source)
{
  return source >> 63;
}

/**
 * assumes that op1 and op2 contain 64 bit two's complement values
 * and returns true if an overflow would occur if they are summed
 * and false otherwise
 *
 * for example, addOverflow(0x8000000000000000, 0x8000000000000000) returns 1
 *              addOverflow(0x7fffffffffffffff, 0x7fffffffffffffff) returns 1
 *              addOverflow(0x8000000000000000, 0x7fffffffffffffff) returns 0
 *
 * @param uint64_t op1 that is one of the operands of the addition
 * @param uint64_t op2 that is the other operand of the addition
 * @return true if op1 + op2 would result in an overflow assuming that op1
 *         and op2 contain 64-bit two's complement values
 *
 * RULES:
 * 1) no loops 
 * 2) you can use other functions you have written, for example, sign
 */
bool Tools::addOverflow(uint64_t op1, uint64_t op2)
{
  //Hint: If an overflow occurs then it overflows by just one bit.
  //      In other words, 65 bits would be needed to store the arithmetic 
  //      result instead of 64 and the sign bit in the stored result (bit 63) is incorrect. 
  //      Thus, the way to check for an overflow is to compare the signs of the
  //      operand and the result.  For example, if you add two positive numbers, 
  //      the result should be positive, otherwise an overflow occurred
  uint64_t x = sign(op1);
  uint64_t y = sign(op2);
  uint64_t z = sign(op1 + op2);
  switch (x + y) {
    case 0 :
      return z == 1;
      break;
    case 1:
      return false;
      break;
    case 2:
      return z == 0;
      break;    
  }
  return true;
  // + + = 0      |  0 0 0
  // + - = either |  1 0 ~
  // - - = 1      |  1 1 1
  // x + y == 1 | x + y + z == 0 | x + y + z == 3
}

/**
 * assumes that op1 and op2 contain 64 bit two's complement values
 * and returns true if an overflow would occur from op2 - op1
 * and false otherwise
 *
 * for example, subOverflow(0x8000000000000000, 0x8000000000000000) returns 0
 *              subOverflow(0x7fffffffffffffff, 0x7fffffffffffffff) returns 0
 *              subOverflow(0x8000000000000000, 0x7fffffffffffffff) returns 1
 *
 * @param uint64_t op1 that is one of the operands of the subtraction
 * @param uint64_t op2 that is the other operand of the subtraction
 * @return true if op2 - op1 would result in an overflow assuming that op1
 *         and op2 contain 64-bit two's complement values
 *
 * RULES:
 * 1) no loops 
 * 2) you can use other functions you have written, for example, sign
 */
bool Tools::subOverflow(uint64_t op1, uint64_t op2)
{
  //See hint for addOverflow
  //Note: you can not simply use addOverflow in this function.  If you negate
  //op1 in order to an add, you may get an overflow.
  if (op1 == 0) return true; // the only overflow number for negation
  uint64_t x = (~op1) + 1;
  return addOverflow(op2,x);
}

/*
 * returns in a 64 bit unsigned the maximum positive number that
 * can be represented when those 64 bits are treated as a 64-bit
 * two's complement value
 *
 * @return uint64_t that contains the bit representation of the
 *                  maximum 64-bit two's complement value
 *
 * RULES
 * 1) no loops or conditionals or switch
 * 2) no casts
 * 3) all constants used may only be between 0 and 255 (inclusive)
 * 4) variables and assignment statements can be used
 * 5) cannot use any of your other functions
*/
uint64_t Tools::maxPos()
{
  uint64_t x = 0;
  return (~x) >> 1;
}

/*
 * returns in a 64 bit unsigned the largest magnitude negative number that
 * can be represented when those 64 bits are treated as a 64-bit
 * two's complement value
 *
 * @return uint64_t that contains the bit representation of the
 *                  minimum 64-bit two's complement value
 *                  (largest magnitude negative value)
 * RULES
 * 1) no loops or conditionals or switch
 * 2) no casts
 * 3) all constants used may only be between 0 and 255 (inclusive)
 * 4) variables and assignment statements can be used
 * 5) cannot use any of your other functions
*/
uint64_t Tools::maxNeg()
{
  uint64_t x = 1;
  return x << 63;
}

/*
 * returns true if its 64 bit unsigned input is the two's complement
 * representation for -1
 *
 * @param uint64_t op that is to be treated as a 64-bit two's complement number
 * @return true if op is -1 if interpreted as a 64-bit two's complement number
 *
 * RULES
 * 1) no loops or conditionals or switch
 * 2) no casts
 * 3) all constants used may only be between 0 and 255 (inclusive)
 * 4) variables and assignment statements can be used
 * 5) only these operators can be used: |, &, <<, >>, !, ~, ^, ||, &&
*/
bool Tools::isNegOne(uint64_t op)
{
  uint64_t x = 0x0;
  return !((~x) ^ op);
}

/*
 * returns true if when adding 1 to its 64 bit unsigned input 
 * it does not result in an overflow when treating that input
 * as a 64-bit two's complement number
 *
 * @param uint64_t op that is to be treated as a 64-bit two's complement number
 * @return true if adding 1 to op would not result in an overflow
 *
 * RULES
 * 1) no loops or conditionals or switch
 * 2) no casts
 * 3) all constants used may only be between 0 and 255 (inclusive)
 * 4) variables and assignment statements can be used
 * 5) only these operators can be used: |, &, <<, >>, !, ~, ^, ||, &&
*/
bool Tools::incOK(uint64_t op)
{
  return op ^ maxPos();
}
/*
 * returns true if when subtracting 1 from its 64 bit unsigned input 
 * it does not result in an overflow when treating that input
 * as a 64-bit two's complement number
 * 
 * @param uint64_t op that is to be treated as a 64-bit two's complement number
 * @return true if subtracting 1 from op would not result in an overflow
 *
 * RULES
 * 1) no loops or conditionals or switch
 * 2) no casts
 * 3) all constants used may only be between 0 and 255 (inclusive)
 * 4) variables and assignment statements can be used
 * 5) only these operators can be used: |, &, <<, >>, !, ~, ^, ||, &&
*/
bool Tools::decOK(uint64_t op)
{
  return op ^ maxNeg();
}

/*
 * takes as input a 64 bit unsigned and returns true if the
 * input is greater than 0 if treated as a 64-bit two's complement
 * number. 
 * 
 * @param uint64_t op that is to be treated as a 64-bit two's complement number
 * @return true if op is greater than 0
 *
 * RULES
 * 1) no loops or conditionals or switch
 * 2) no casts
 * 3) all constants used may only be between 0 and 255 (inclusive)
 * 4) variables and assignment statements can be used
 * 5) only these operators can be used: |, &, <<, >>, !, ~, ^, ||, &&
*/
bool Tools::isGreaterThan0(uint64_t op)
{
  return (!sign(op)) && op;
}
