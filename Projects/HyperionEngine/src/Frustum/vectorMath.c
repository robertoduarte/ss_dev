#include "vectorMath.h"

static const FIXED fxMaximum = 0x7FFFFFFF; /*!< the maximum value of FIXED */
static const FIXED fxMinimum = 0x80000000; /*!< the minimum value of FIXED */
static const FIXED fxOverflow = 0x80000000;

FxVector fxVectorAdd(FxVector a, FxVector b)
{
    return (FxVector){a.x + b.x, a.y + b.y, a.z + b.z};
}

FxVector fxVectorSub(FxVector a, FxVector b)
{
    return (FxVector){a.x - b.x, a.y - b.y, a.z - b.z};
}

FxVector fxVectorCross(FxVector a, FxVector b)
{
    return (FxVector){(slMulFX(a.z, b.y) - slMulFX(a.y, b.z)),
                      -(slMulFX(a.x, b.z) - slMulFX(a.z, b.x)),
                      (slMulFX(a.y, b.x) - slMulFX(a.x, b.y))};
}

FxVector fxVectorMult(FxVector a, FIXED b)
{
    return (FxVector){slMulFX(a.x, b), slMulFX(a.y, b), slMulFX(a.z, b)};
}

FxVector fxVectorDiv(FxVector a, FIXED b)
{
    return (FxVector){slDivFX(a.x, b), slDivFX(a.y, b), slDivFX(a.z, b)};
}

FIXED fxScale(FIXED value, char scale)
{
    if (scale > 0)
    {
        FIXED temp = value << scale;
        if (temp >> scale != value)
        {
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

static unsigned char clz(unsigned x)
{
    unsigned char result = 0;
    if (x == 0)
        return 32;
    while (!(x & 0xF0000000))
    {
        result += 4;
        x <<= 4;
    }
    while (!(x & 0x80000000))
    {
        result += 1;
        x <<= 1;
    }
    return result;
}

static unsigned char ilog2(unsigned char v)
{
    unsigned char result = 0;
    if (v & 0xF0)
    {
        result += 4;
        v >>= 4;
    }
    while (v)
    {
        result++;
        v >>= 1;
    }
    return result;
}

FIXED fxVectorLength(FxVector vector)
{
    FIXED *a = &vector.x;
    unsigned char a_stride = &vector.y - &vector.x;
    unsigned char n = 3;

    FIXED sum = 0;
    FIXED max = 0;

    // Calculate inclusive OR of all values to find out the maximum.
    {
        unsigned char i;
        const FIXED *p = (FIXED *)a;
        for (i = 0; i < n; i++, p += a_stride)
        {
            max |= ABS(*p);
        }
    }

    // To avoid overflows, the values before squaring can be max 128.0,
    // i.e. v & 0xFF800000 must be 0. Also, to avoid overflow in sum,
    // we need additional log2(n) bits of space.
    unsigned char scale = clz(max) - 9 - ilog2(n) / 2;

    while (n--)
    {
        FIXED val = fxScale(*a, scale);
        FIXED product = slMulFX(val, val);
        sum = sum + product;

        a += a_stride;
    }

    if (sum == fxOverflow)
    {
        return sum;
    }

    FIXED result = slSquartFX(sum);
    return fxScale(result, -scale);
}

FxVector fxVectorNormalize(FxVector vector)
{
    FIXED length = fxVectorLength(vector);

    if (length)
    {
        return (FxVector){slDivFX(length, vector.x),
                          slDivFX(length, vector.y),
                          slDivFX(length, vector.z)};
    }
    else
    {
        return (FxVector){0, 0, 0};
    }
}
