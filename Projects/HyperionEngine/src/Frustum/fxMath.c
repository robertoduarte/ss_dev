#include "fxMath.h"

FIXED fxAdd(FIXED a, FIXED b)
{
    // Use unsigned integers because overflow with signed integers is
    // an undefined operation (http://www.airs.com/blog/archives/120).
    unsigned _a = a, _b = b;
    unsigned sum = _a + _b;

    // Overflow can only happen if sign of a == sign of b, and then
    // it causes sign of sum != sign of a.
    if (!((_a ^ _b) & 0x80000000) && ((_a ^ sum) & 0x80000000))
        return fxOverflow;

    return sum;
}

FIXED fxSub(FIXED a, FIXED b)
{
    unsigned _a = a, _b = b;
    unsigned diff = _a - _b;

    // Overflow can only happen if sign of a != sign of b, and then
    // it causes sign of diff != sign of a.
    if (((_a ^ _b) & 0x80000000) && ((_a ^ diff) & 0x80000000))
        return fxOverflow;

    return diff;
}

FIXED fxMult(FIXED a, FIXED b)
{
    // Each argument is divided to 16-bit parts.
    //					AB
    //			*	 CD
    // -----------
    //					BD	16 * 16 -> 32 bit products
    //				 CB
    //				 AD
    //				AC
    //			 |----| 64 bit product
    int A = (a >> 16), C = (b >> 16);
    unsigned B = (a & 0xFFFF), D = (b & 0xFFFF);

    int AC = A*C;
    int AD_CB = A * D + C*B;
    unsigned BD = B*D;

    int product_hi = AC + (AD_CB >> 16);

    // Handle carry from lower 32 bits to upper part of result.
    unsigned ad_cb_temp = AD_CB << 16;
    unsigned product_lo = BD + ad_cb_temp;
    if (product_lo < BD)
        product_hi++;


    // The upper 17 bits should all be the same (the sign).
    if (product_hi >> 31 != product_hi >> 15)
        return fxOverflow;

    // Subtracting 0x8000 (= 0.5) and then using signed right shift
    // achieves proper rounding to result-1, except in the corner
    // case of negative numbers and lowest word = 0x8000.
    // To handle that, we also have to subtract 1 for negative numbers.
    unsigned product_lo_tmp = product_lo;
    product_lo -= 0x8000;
    product_lo -= (unsigned) product_hi >> 31;
    if (product_lo > product_lo_tmp)
        product_hi--;

    // Discard the lowest 16 bits. Note that this is not exactly the same
    // as dividing by 0x10000. For example if product = -1, result will
    // also be -1 and not 0. This is compensated by adding +1 to the result
    // and compensating this in turn in the rounding above.
    FIXED result = (product_hi << 16) | (product_lo >> 16);
    result += 1;
    //return result;
    return slMulFX(a, b);
}

FIXED fxDiv(FIXED a, FIXED b)
{
    // This uses the basic binary restoring division algorithm.
    // It appears to be faster to do the whole division manually than
    // trying to compose a 64-bit divide out of 32-bit divisions on
    // platforms without hardware divide.

    if (a == 0)
        return fxMinimum;

    unsigned remainder = (b >= 0) ? b : (-b);
    unsigned divider = (a >= 0) ? a : (-a);

    unsigned quotient = 0;
    unsigned bit = 0x10000;

    /* The algorithm requires D >= R */
    while (divider < remainder)
    {
        divider <<= 1;
        bit <<= 1;
    }

    if (!bit)
        return fxOverflow;

    if (divider & 0x80000000)
    {
        // Perform one step manually to avoid overflows later.
        // We know that divider's bottom bit is 0 here.
        if (remainder >= divider)
        {
            quotient |= bit;
            remainder -= divider;
        }
        divider >>= 1;
        bit >>= 1;
    }

    /* Main division loop */
    while (bit && remainder)
    {
        if (remainder >= divider)
        {
            quotient |= bit;
            remainder -= divider;
        }

        remainder <<= 1;
        bit >>= 1;
    }

    if (remainder >= divider)
    {
        quotient++;
    }

    FIXED result = quotient;

    /* Figure out the sign of result */
    if ((b ^ a) & 0x80000000)
    {
        if (result == fxMinimum)
            return fxOverflow;

        result = -result;
    }
    return result;
}

FIXED fxScale(FIXED value, char scale)
{
    if (scale > 0)
    {
        FIXED temp = value << scale;
        if (temp >> scale != value)
        {
            slPrint("OVERFLOW!", slLocate(0, 2));
            return fxOverflow;
        }
        else
            return temp;
    }
    else if (scale < 0)
    {
        return value >> -scale;
    }
    else
    {
        return value;
    }
}

FIXED fxSqrt(FIXED value)
{
    unsigned char neg = (value < 0);
    unsigned num = (neg ? -value : value);
    unsigned result = 0;
    unsigned bit;
    unsigned char n;

    // Many numbers will be less than 15, so
    // this gives a good balance between time spent
    // in if vs. time spent in the while loop
    // when searching for the starting value.
    if (num & 0xFFF00000)
        bit = (unsigned) 1 << 30;
    else
        bit = (unsigned) 1 << 18;

    while (bit > num) bit >>= 2;

    // The main part is executed twice, in order to avoid
    // using 64 bit values in computations.
    for (n = 0; n < 2; n++)
    {
        // First we get the top 24 bits of the answer.
        while (bit)
        {
            if (num >= result + bit)
            {
                num -= result + bit;
                result = (result >> 1) + bit;
            }
            else
            {
                result = (result >> 1);
            }
            bit >>= 2;
        }

        if (n == 0)
        {
            // Then process it again to get the lowest 8 bits.
            if (num > 65535)
            {
                // The remainder 'num' is too large to be shifted left
                // by 16, so we have to add 1 to result manually and
                // adjust 'num' accordingly.
                // num = a - (result + 0.5)^2
                //	 = num + result^2 - (result + 0.5)^2
                //	 = num - result - 0.5
                num -= result;
                num = (num << 16) - 0x8000;
                result = (result << 16) + 0x8000;
            }
            else
            {
                num <<= 16;
                result <<= 16;
            }

            bit = 1 << 14;
        }
    }

    // Finally, if next bit would have been 1, round the result upwards.
    if (num > result)
    {
        result++;
    }

    return (neg ? -(FIXED) result : (FIXED) result);
}


