#include "vectorMath.h"

void fx3dSet(FIXED result[XYZ], FIXED x, FIXED y, FIXED z)
{
    result[X] = x;
    result[Y] = y;
    result[Z] = z;
}

void fx3dCopy(FIXED result[XYZ], FIXED source[XYZ])
{
    result[X] = source[X];
    result[Y] = source[Y];
    result[Z] = source[Z];
}

void fxVectorAdd(VECTOR dest, VECTOR a, VECTOR b)
{
    dest[X] = fxAdd(a[X], b[X]);
    dest[Y] = fxAdd(a[Y], b[Y]);
    dest[Z] = fxAdd(a[Z], b[Z]);
}

void fxVectorSub(VECTOR dest, VECTOR a, VECTOR b)
{
    dest[X] = fxSub(a[X], b[X]);
    dest[Y] = fxSub(a[Y], b[Y]);
    dest[Z] = fxSub(a[Z], b[Z]);
}

void fxVectorCross(VECTOR dest, VECTOR a, VECTOR b)
{
    dest[X] = fxSub(fxMult(a[Z], b[Y]), fxMult(a[Y], b[Z]));
    dest[Y] = -fxSub(fxMult(a[X], b[Z]), fxMult(a[Z], b[X]));
    dest[Z] = fxSub(fxMult(a[Y], b[X]), fxMult(a[X], b[Y]));
}

void fxVectorMult(VECTOR dest, VECTOR a, FIXED b)
{
    dest[X] = fxMult(a[X], b);
    dest[Y] = fxMult(a[Y], b);
    dest[Z] = fxMult(a[Z], b);
}

void fxVectorDiv(VECTOR dest, VECTOR a, FIXED b)
{
    dest[X] = fxDiv(a[X], b);
    dest[Y] = fxDiv(a[Y], b);
    dest[Z] = fxDiv(a[Z], b);
}

static unsigned char clz(unsigned x)
{
    unsigned char result = 0;
    if (x == 0) return 32;
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

FIXED fxVectorLength(VECTOR vector)
{
    FIXED *a = &vector[X];
    unsigned char a_stride = &vector[Y] - &vector[X];
    unsigned char n = 3;

    FIXED sum = 0;
    FIXED max = 0;

    // Calculate inclusive OR of all values to find out the maximum.
    {
        unsigned char i;
        const FIXED *p = (FIXED*) a;
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
        FIXED product = fxMult(val, val);
        sum = fxAdd(sum, product);

        a += a_stride;
    }

    if (sum == fxOverflow)
    {
        return sum;
    }

    FIXED result = fxSqrt(sum);
    return fxScale(result, -scale);
}

void fxVectorNormalize(VECTOR vector)
{
    FIXED length = fxVectorLength(vector);

    if (length)
    {
        vector[X] = fxDiv(length, vector[X]);
        vector[Y] = fxDiv(length, vector[Y]);
        vector[Z] = fxDiv(length, vector[Z]);
    }
    else
    {
        vector[X] = 0;
        vector[Y] = 0;
        vector[Z] = 0;
    }
}

FIXED fxVectorInnerProduct(VECTOR a, VECTOR b)
{
    FIXED auxiliarVector;
    auxiliarVector = fxMult(a[X], b[X]);
    auxiliarVector = fxAdd(auxiliarVector, fxMult(a[Y], b[Y]));
    auxiliarVector = fxAdd(auxiliarVector, fxMult(a[Z], b[Z]));
    return auxiliarVector;
}
