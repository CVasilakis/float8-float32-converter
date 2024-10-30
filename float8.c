#include "float8.h"

/****************************
 * EXPONENT_BIAS DEFINITION *
 ****************************/

#if EXPONENT_BITS == 1
#define EXPONENT_BIAS 0
#endif

#if EXPONENT_BITS == 2
#define EXPONENT_BIAS 1
#endif

#if EXPONENT_BITS == 3
#define EXPONENT_BIAS 3
#endif

#if EXPONENT_BITS == 4
#define EXPONENT_BIAS 7
#endif

#if EXPONENT_BITS == 5
#define EXPONENT_BIAS 15
#endif

#if EXPONENT_BITS == 6
#define EXPONENT_BIAS 31
#endif

/****************************
 * FUNCTION IMPLEMENTATIONS *
 ****************************/

unsigned char float8_to_uchar(float8_t f8) { return *(unsigned char *)&f8; }

float8_t uchar_to_float8(unsigned char c) { return *(float8_t *)&c; }

/*
 * Increase a float8_t number to the next representable value.
 *
 * By default the float8 numbers are rounded down to the nearest
 * representable value. When needed, the number can be rounded up
 * to the next representable value. Due to the format of the 
 * float8_t number, this can be easily done by increasing the 
 * unsigned char representation of the float8 number by one.
 *
 * @param c: unsigned char to be converted
 * @return: 8-bit minifloat (float8_t) number
 */
float8_t increse_by_one(float8_t f8) {
    unsigned char temp = float8_to_uchar(f8);
    temp++;
    return uchar_to_float8(temp);
}

float8_t float_to_float8(float f) {
    float8_t result;
    uint32_t *p = (uint32_t *)&f;

    // Extract sign, exponent and fraction from the float
    uint32_t sign = (*p >> 31) & 0x1;
    uint32_t exponent = (*p >> 23) & 0xFF;
    uint32_t fraction = *p & 0x7FFFFF;

    // Save the sign
    result.sign = sign;

#if EXPONENT_BITS == 1
    if (exponent == 0x0) {          // Zero case (float32 subnormal are zero in float8)
        result.exponent = 0;        // 0
        result.fraction = 0;        // 00 0000
    } else if (exponent == 0xFF) {  // Infinity or NaN case
        result.exponent = 0x1;      // 1
        result.fraction = 0;        // 00 0000 - Only Infinity is represented
    } else {
        int32_t exp_value = exponent - 127;
        if (exp_value >= 1) {  // Infinity case
            result.exponent = 0x1;
            result.fraction = 0;
        } else if (exp_value <= -7) {  // Zero case
            result.exponent = 0;
            result.fraction = 0;
        } else if (exp_value == -6) {  // Subnormal case
            result.exponent = 0;
            result.fraction = 0;
            // (Rounding) Increase by one
            if (((fraction >> 22) & 0x01) == 1) {
                result = increse_by_one(result);
            }
        } else if (exp_value == -5) {  // Subnormal case
            result.exponent = 0;
            result.fraction = 0b000001;
            // (Rounding) Increase by one
            if (((fraction >> 22) & 0x01) == 1) {
                result = increse_by_one(result);
            }
        } else if (exp_value == -4) {  // Subnormal case
            result.exponent = 0;
            result.fraction = 0b000010 | ((fraction >> 22) & 0x01);
            // (Rounding) Increase by one
            if (((fraction >> 21) & 0x01) == 1) {
                result = increse_by_one(result);
            }
        } else if (exp_value == -3) {  // Subnormal case
            result.exponent = 0;
            result.fraction = 0b000100 | ((fraction >> 21) & 0x03);
            // (Rounding) Increase by one
            if (((fraction >> 20) & 0x01) == 1) {
                result = increse_by_one(result);
            }
        } else if (exp_value == -2) {  // Subnormal case
            result.exponent = 0;
            result.fraction = 0b001000 | ((fraction >> 20) & 0x07);
            // (Rounding) Increase by one
            if (((fraction >> 19) & 0x01) == 1) {
                result = increse_by_one(result);
            }
        } else if (exp_value == -1) {  // Subnormal case
            result.exponent = 0;
            result.fraction = 0b010000 | ((fraction >> 19) & 0x0F);
            // (Rounding) Increase by one
            if (((fraction >> 18) & 0x01) == 1) {
                result = increse_by_one(result);
            }
        } else if (exp_value == 0) {  // Subnormal case
            result.exponent = 0;
            result.fraction = 0b100000 | ((fraction >> 18) & 0x1F);
            // (Rounding) Increase by one
            if (((fraction >> 17) & 0x01) == 1) {
                result = increse_by_one(result);
            }
        } 
        // NO normal case for 1-1-6
        // The 1-1-6 float8 format DOES NOT have normal values
    }
#endif

#if EXPONENT_BITS == 2
    if (exponent == 0x0) {          // Zero case (float32 subnormal are zero in float8)
        result.exponent = 0;        // 00
        result.fraction = 0;        // 0 0000
    } else if (exponent == 0xFF) {  // Infinity or NaN case
        result.exponent = 0x3;      // 11
        result.fraction = 0;        // 0 0000 - Only Infinity is represented
    } else {
        int32_t exp_value = exponent - 127;
        if (exp_value >= 2) {  // Infinity case
            result.exponent = 0x3;
            result.fraction = 0;
        } else if (exp_value <= -7) {  // Zero case
            result.exponent = 0;
            result.fraction = 0;
        } else if (exp_value == -6) {  // Subnormal case
            result.exponent = 0;
            result.fraction = 0;
            // (Rounding) Increase by one
            if (((fraction >> 22) & 0x01) == 1) {
                result = increse_by_one(result);
            }
        } else if (exp_value == -5) {  // Subnormal case
            result.exponent = 0;
            result.fraction = 0b00001;
            // (Rounding) Increase by one
            if (((fraction >> 22) & 0x01) == 1) {
                result = increse_by_one(result);
            }
        } else if (exp_value == -4) {  // Subnormal case
            result.exponent = 0;
            result.fraction = 0b00010 | ((fraction >> 22) & 0x01);
            // (Rounding) Increase by one
            if (((fraction >> 21) & 0x01) == 1) {
                result = increse_by_one(result);
            }
        } else if (exp_value == -3) {  // Subnormal case
            result.exponent = 0;
            result.fraction = 0b00100 | ((fraction >> 21) & 0x03);
            // (Rounding) Increase by one
            if (((fraction >> 20) & 0x01) == 1) {
                result = increse_by_one(result);
            }
        } else if (exp_value == -2) {  // Subnormal case
            result.exponent = 0;
            result.fraction = 0b01000 | ((fraction >> 20) & 0x07);
            // (Rounding) Increase by one
            if (((fraction >> 19) & 0x01) == 1) {
                result = increse_by_one(result);
            }
        } else if (exp_value == -1) {  // Subnormal case
            result.exponent = 0;
            result.fraction = 0b10000 | ((fraction >> 19) & 0x0F);
            // (Rounding) Increase by one
            if (((fraction >> 18) & 0x01) == 1) {
                result = increse_by_one(result);
            }
        } else {  // Normal case
            result.exponent = exp_value + EXPONENT_BIAS;
            result.fraction = fraction >> 18;  // Only keep 5 bits of fraction
            // (Rounding) Increase by one
            if (((fraction >> 17) & 0x01) == 1) {
                result = increse_by_one(result);
            }
        }
    }
#endif

#if EXPONENT_BITS == 3
    if (exponent == 0x0) {          // Zero case (float32 subnormal are zero in float8)
        result.exponent = 0;        // 000
        result.fraction = 0;        // 0000
    } else if (exponent == 0xFF) {  // Infinity or NaN case
        result.exponent = 0x7;      // 111
        result.fraction = 0;        // 0000 - Only Infinity is represented
    } else {
        int32_t exp_value = exponent - 127;
        if (exp_value >= 4) {  // Infinity case
            result.exponent = 0x7;
            result.fraction = 0;
        } else if (exp_value <= -8) {  // Zero case
            result.exponent = 0;
            result.fraction = 0;
        } else if (exp_value == -7) {  // Subnormal case
            result.exponent = 0;
            result.fraction = 0;
            // (Rounding) Increase by one
            if (((fraction >> 22) & 0x01) == 1) {
                result = increse_by_one(result);
            }
        } else if (exp_value == -6) {  // Subnormal case
            result.exponent = 0;
            result.fraction = 0b0001;
            // (Rounding) Increase by one
            if (((fraction >> 22) & 0x01) == 1) {
                result = increse_by_one(result);
            }
        } else if (exp_value == -5) {  // Subnormal case
            result.exponent = 0;
            result.fraction = 0b0010 | ((fraction >> 22) & 0x01);
            // (Rounding) Increase by one
            if (((fraction >> 21) & 0x01) == 1) {
                result = increse_by_one(result);
            }
        } else if (exp_value == -4) {  // Subnormal case
            result.exponent = 0;
            result.fraction = 0b0100 | ((fraction >> 21) & 0x03);
            // (Rounding) Increase by one
            if (((fraction >> 20) & 0x01) == 1) {
                result = increse_by_one(result);
            }
        } else if (exp_value == -3) {  // Subnormal case
            result.exponent = 0;
            result.fraction = 0b1000 | ((fraction >> 20) & 0x07);
            // (Rounding) Increase by one
            if (((fraction >> 19) & 0x01) == 1) {
                result = increse_by_one(result);
            }
        } else {  // Normal case
            result.exponent = exp_value + EXPONENT_BIAS;
            result.fraction = fraction >> 19;  // Only keep 4 bits of fraction
            // (Rounding) Increase by one
            if (((fraction >> 18) & 0x01) == 1) {
                result = increse_by_one(result);
            }
        }
    }
#endif

#if EXPONENT_BITS == 4
    if (exponent == 0x0) {          // Zero case (float32 subnormal are zero in float8)
        result.exponent = 0;        // 0000
        result.fraction = 0;        // 000
    } else if (exponent == 0xFF) {  // Infinity or NaN case
        result.exponent = 0xF;      // 1111
        result.fraction = 0;        // 000 - Only Infinity is represented
    } else {
        int32_t exp_value = exponent - 127;
        if (exp_value >= 8) {  // Infinity case
            result.exponent = 0xF;
            result.fraction = 0;
        } else if (exp_value <= -11) {  // Zero case
            result.exponent = 0;
            result.fraction = 0;
        } else if (exp_value == -10) {  // Subnormal case
            result.exponent = 0;
            result.fraction = 0;
            // (Rounding) Increase by one
            if (((fraction >> 22) & 0x01) == 1) {
                result = increse_by_one(result);
            }
        } else if (exp_value == -9) {  // Subnormal case
            result.exponent = 0;
            result.fraction = 0b001;
            // (Rounding) Increase by one
            if (((fraction >> 22) & 0x01) == 1) {
                result = increse_by_one(result);
            }
        } else if (exp_value == -8) {  // Subnormal case
            result.exponent = 0;
            result.fraction = 0b010 | ((fraction >> 22) & 0x01);
            // (Rounding) Increase by one
            if (((fraction >> 21) & 0x01) == 1) {
                result = increse_by_one(result);
            }
        } else if (exp_value == -7) {  // Subnormal case
            result.exponent = 0;
            result.fraction = 0b100 | ((fraction >> 21) & 0x03);
            // (Rounding) Increase by one
            if (((fraction >> 20) & 0x01) == 1) {
                result = increse_by_one(result);
            }
        } else {  // Normal case
            result.exponent = exp_value + EXPONENT_BIAS;
            result.fraction = fraction >> 20;  // Only keep 3 bits of fraction
            // (Rounding) Increase by one
            if (((fraction >> 19) & 0x01) == 1) {
                result = increse_by_one(result);
            }
        }
    }
#endif

#if EXPONENT_BITS == 5
    if (exponent == 0x0) {          // Zero case (float32 subnormal are zero in float8)
        result.exponent = 0;        // 0 0000
        result.fraction = 0;        // 00
    } else if (exponent == 0xFF) {  // Infinity or NaN case
        result.exponent = 0x1F;     // 1 1111
        result.fraction = 0;        // 00 - Only Infinity is represented
    } else {
        int32_t exp_value = exponent - 127;
        if (exp_value >= 16) {  // Infinity case
            result.exponent = 0x1F;
            result.fraction = 0;
        } else if (exp_value <= -18) {  // Zero case
            result.exponent = 0;
            result.fraction = 0;
        } else if (exp_value == -17) {  // Subnormal case
            result.exponent = 0;
            result.fraction = 0;
            // (Rounding) Increase by one
            if (((fraction >> 22) & 0x01) == 1) {
                result = increse_by_one(result);
            }
        } else if (exp_value == -16) {  // Subnormal case
            result.exponent = 0;
            result.fraction = 0b01;
            // (Rounding) Increase by one
            if (((fraction >> 22) & 0x01) == 1) {
                result = increse_by_one(result);
            }
        } else if (exp_value == -15) {  // Subnormal case
            result.exponent = 0;
            result.fraction = 0b10 | ((fraction >> 22) & 0x01);
            // (Rounding) Increase by one
            if (((fraction >> 21) & 0x01) == 1) {
                result = increse_by_one(result);
            }
        } else {  // Normal case
            result.exponent = exp_value + EXPONENT_BIAS;
            result.fraction = fraction >> 21;  // Only keep 2 bits of fraction
            // (Rounding) Increase by one
            if (((fraction >> 20) & 0x01) == 1) {
                result = increse_by_one(result);
            }
        }
    }
#endif

#if EXPONENT_BITS == 6
    if (exponent == 0x0) {          // Zero case (float32 subnormal are zero in float8)
        result.exponent = 0;        // 00 0000
        result.fraction = 0;        // 0
    } else if (exponent == 0xFF) {  // Infinity or NaN case
        result.exponent = 0x3F;     // 11 1111
        result.fraction = 0;        // 0 - Only Infinity is represented
    } else {
        int32_t exp_value = exponent - 127;
        if (exp_value >= 32) {  // Infinity case
            result.exponent = 0x3F;
            result.fraction = 0;
        } else if (exp_value <= -33) {  // Zero case
            result.exponent = 0;
            result.fraction = 0;
        } else if (exp_value == -32) {  // Subnormal case
            result.exponent = 0;
            result.fraction = 0;
            // (Rounding) Increase by one
            if (((fraction >> 22) & 0x01) == 1) {
                result = increse_by_one(result);
            }
        } else if (exp_value == -31) {  // Subnormal case
            result.exponent = 0;
            result.fraction = 0b1;
            // (Rounding) Increase by one
            if (((fraction >> 22) & 0x01) == 1) {
                result = increse_by_one(result);
            }
        } else {  // Normal case
            result.exponent = exp_value + EXPONENT_BIAS;
            result.fraction = fraction >> 22;  // Only keep 1 bit (MSB) of fraction
            // (Rounding) Increase by one
            if (((fraction >> 21) & 0x01) == 1) {
                result = increse_by_one(result);
            }
        }
    }
#endif

    return result;
}

float float8_to_float(float8_t f8) {
    uint32_t sign = f8.sign;
    uint32_t exponent = 0;
    uint32_t fraction = 0;

#if EXPONENT_BITS == 1
    if (f8.exponent == 0x0) {    // Zero or subnormal case
        if (f8.fraction == 0) {  // Zero case
            exponent = 0;
            fraction = 0;
        } else if (f8.fraction >> 5 == 1) {  // Subnormal case
            exponent = 127;
            fraction = (f8.fraction & 0x1F) << 18;
        } else if (f8.fraction >> 4 == 1) {  // Subnormal case
            exponent = 126;
            fraction = (f8.fraction & 0x0F) << 19;
        } else if (f8.fraction >> 3 == 1) {  // Subnormal case
            exponent = 125;
            fraction = (f8.fraction & 0x07) << 20;
        } else if (f8.fraction >> 2 == 1) {  // Subnormal case
            exponent = 124;
            fraction = (f8.fraction & 0x03) << 21;
        } else if (f8.fraction >> 1 == 1) {  // Subnormal case
            exponent = 123;
            fraction = (f8.fraction & 0x01) << 22;
        } else if (f8.fraction == 1) {  // Subnormal case
            exponent = 122;
            fraction = 0;
        }
    } else if ((f8.exponent == 0x1)) {  // Infinity case - No NaN implemented for float8
        exponent = 0xFF;                // 1111 1111
        fraction = 0;
    }
    // NO normal case for 1-1-6
    // The 1-1-6 float8 format DOES NOT have normal values
#endif

#if EXPONENT_BITS == 2
    if (f8.exponent == 0x0) {    // Zero or subnormal case
        if (f8.fraction == 0) {  // Zero case
            exponent = 0;
            fraction = 0;
        } else if (f8.fraction >> 4 == 1) {  // Subnormal case
            exponent = 126;
            fraction = (f8.fraction & 0x0F) << 19;
        } else if (f8.fraction >> 3 == 1) {  // Subnormal case
            exponent = 125;
            fraction = (f8.fraction & 0x07) << 20;
        } else if (f8.fraction >> 2 == 1) {  // Subnormal case
            exponent = 124;
            fraction = (f8.fraction & 0x03) << 21;
        } else if (f8.fraction >> 1 == 1) {  // Subnormal case
            exponent = 123;
            fraction = (f8.fraction & 0x01) << 22;
        } else if (f8.fraction == 1) {  // Subnormal case
            exponent = 122;
            fraction = 0;
        }
    } else if ((f8.exponent == 0x3)) {  // Infinity case - No NaN implemented for float8
        exponent = 0xFF;                // 1111 1111
        fraction = 0;
    } else {  // Normal case
        exponent = (f8.exponent - EXPONENT_BIAS + 127);
        fraction = f8.fraction << 18;
    }
#endif

#if EXPONENT_BITS == 3
    if (f8.exponent == 0x0) {    // Zero or subnormal case
        if (f8.fraction == 0) {  // Zero case
            exponent = 0;
            fraction = 0;
        } else if (f8.fraction >> 3 == 1) {  // Subnormal case
            exponent = 124;
            fraction = (f8.fraction & 0x07) << 20;
        } else if (f8.fraction >> 2 == 1) {  // Subnormal case
            exponent = 123;
            fraction = (f8.fraction & 0x03) << 21;
        } else if (f8.fraction >> 1 == 1) {  // Subnormal case
            exponent = 122;
            fraction = (f8.fraction & 0x01) << 22;
        } else if (f8.fraction == 1) {  // Subnormal case
            exponent = 121;
            fraction = 0;
        }
    } else if (f8.exponent == 0x7) {  // Infinity case - No NaN implemented for float8
        exponent = 0xFF;              // 1111 1111
        fraction = 0;
    } else {  // Normal case
        exponent = (f8.exponent - EXPONENT_BIAS + 127);
        fraction = f8.fraction << 19;
    }
#endif

#if EXPONENT_BITS == 4
    if (f8.exponent == 0x0) {    // Zero or subnormal case
        if (f8.fraction == 0) {  // Zero case
            exponent = 0;
            fraction = 0;
        } else if (f8.fraction >> 2 == 1) {  // Subnormal case
            exponent = 120;
            fraction = (f8.fraction & 0x03) << 21;
        } else if (f8.fraction >> 1 == 1) {  // Subnormal case
            exponent = 119;
            fraction = (f8.fraction & 0x01) << 22;
        } else if (f8.fraction == 1) {  // Subnormal case
            exponent = 118;
            fraction = 0;
        }
    } else if (f8.exponent == 0xF) {  // Infinity case - No NaN implemented for float8
        exponent = 0xFF;              // 1111 1111
        fraction = 0;
    } else {  // Normal case
        exponent = (f8.exponent - EXPONENT_BIAS + 127);
        fraction = f8.fraction << 20;
    }
#endif

#if EXPONENT_BITS == 5
    if (f8.exponent == 0x0) {    // Zero or subnormal case
        if (f8.fraction == 0) {  // Zero case
            exponent = 0;
            fraction = 0;
        } else if (f8.fraction >> 1 == 1) {  // Subnormal case
            exponent = 112;
            fraction = (f8.fraction & 0x01) << 22;
        } else if (f8.fraction == 1) {  // Subnormal case
            exponent = 111;
            fraction = 0;
        }
    } else if (f8.exponent == 0x1F) {  // Infinity case - No NaN implemented for float8
        exponent = 0xFF;               // 1111 1111
        fraction = 0;
    } else {  // Normal case
        exponent = (f8.exponent - EXPONENT_BIAS + 127);
        fraction = f8.fraction << 21;
    }
#endif

#if EXPONENT_BITS == 6
    if (f8.exponent == 0x0) {    // Zero or subnormal case
        if (f8.fraction == 0) {  // Zero case
            exponent = 0;
            fraction = 0;
        } else if (f8.fraction == 1) {  // Subnormal case
            exponent = 96;
            fraction = 0;
        }
    } else if (f8.exponent == 0x3F) {  // Infinity case - No NaN implemented for float8
        exponent = 0xFF;               // 1111 1111
        fraction = 0;
    } else {  // Normal case
        exponent = (f8.exponent - EXPONENT_BIAS + 127);
        fraction = f8.fraction << 22;
    }
#endif

    // Concatenate the sign, exponent and fraction to get the float
    uint32_t result = (sign << 31) | (exponent << 23) | fraction;

    // Cast to float to return the correct data type
    return *(float *)&result;
}
