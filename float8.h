#ifndef FLOAT8_H
#define FLOAT8_H

#include <stdint.h>

/***********************************************************
 * The number of bits used to represent the float8 exponent.
 ***********************************************************/
#define EXPONENT_BITS 4

/*
 * This is the 8-bit minifloat representation.
 * The float8_t struct is used to represent a float8 number.
 * Bitfields used to limit the size of the float8_t struct to 8 bits.
 * 
 * CAUTION: Need to take care of the target system's endianness.
 */
typedef struct {
    uint8_t fraction : 7 - EXPONENT_BITS;
    uint8_t exponent : EXPONENT_BITS;
    uint8_t sign : 1;
} float8_t;

/* Convert a number from single-precision floating point
 * number to 8-bit minifloat (float8_t) number.
 *
 * @param f: single-precision floating point number to be converted
 * @return: 8-bit minifloat (float8_t) number
 */
float8_t float_to_float8(float f);

/* Convert a number from 8-bit minifloat (float8_t) number
 * to single-precision floating point number.
 *
 * @param f8: 8-bit minifloat (float8_t) number to be converted
 * @return: single-precision floating point number
 */
float float8_to_float(float8_t f8);

/* Convert an unsigned char to a float8_t.
 * WARNING: type punning violates the strict aliasing rule.
 *
 * @param c: unsigned char to be converted
 * @return: 8-bit minifloat (float8_t) number
 */
float8_t uchar_to_float8(unsigned char c);

/* Convert a float8_t to an unsigned char.
 * WARNING: type punning violates the strict aliasing rule.
 *
 * @param f8: 8-bit minifloat (float8_t) number to be converted
 * @return: unsigned char
 */
unsigned char float8_to_uchar(float8_t f8);

#endif
