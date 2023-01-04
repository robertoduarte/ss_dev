#include "Trigonometry.h"

#include "stddef.h"
#include "stdbool.h"

#define FLOAT_TO_FIXED_POINT(f) (fix16_t)(f >= 0 ? f * 65536.0f + 0.5f : f * 65536.0f - 0.5f)

#define LOKUP_CACHE_TYPE(T)          \
    struct                           \
    {                                \
        T value;                     \
        T interpolationMultiplicand; \
    }

const LOKUP_CACHE_TYPE(fix16_t) sinTable[] = {{FLOAT_TO_FIXED_POINT(0.000000f), 205556},
                                              {FLOAT_TO_FIXED_POINT(0.098017f), 203577},
                                              {FLOAT_TO_FIXED_POINT(0.195090f), 199637},
                                              {FLOAT_TO_FIXED_POINT(0.290285f), 193774},
                                              {FLOAT_TO_FIXED_POINT(0.382683f), 186045},
                                              {FLOAT_TO_FIXED_POINT(0.471397f), 176524},
                                              {FLOAT_TO_FIXED_POINT(0.555570f), 165303},
                                              {FLOAT_TO_FIXED_POINT(0.634393f), 152491},
                                              {FLOAT_TO_FIXED_POINT(0.707107f), 138210},
                                              {FLOAT_TO_FIXED_POINT(0.773010f), 122597},
                                              {FLOAT_TO_FIXED_POINT(0.831470f), 105804},
                                              {FLOAT_TO_FIXED_POINT(0.881921f), 87992},
                                              {FLOAT_TO_FIXED_POINT(0.923880f), 69333},
                                              {FLOAT_TO_FIXED_POINT(0.956940f), 50006},
                                              {FLOAT_TO_FIXED_POINT(0.980785f), 30197},
                                              {FLOAT_TO_FIXED_POINT(0.995185f), 10098},
                                              {FLOAT_TO_FIXED_POINT(1.000000f), -10098},
                                              {FLOAT_TO_FIXED_POINT(0.995185f), -30197},
                                              {FLOAT_TO_FIXED_POINT(0.980785f), -50006},
                                              {FLOAT_TO_FIXED_POINT(0.956940f), -69333},
                                              {FLOAT_TO_FIXED_POINT(0.923880f), -87992},
                                              {FLOAT_TO_FIXED_POINT(0.881921f), -105804},
                                              {FLOAT_TO_FIXED_POINT(0.831470f), -122597},
                                              {FLOAT_TO_FIXED_POINT(0.773010f), -138210},
                                              {FLOAT_TO_FIXED_POINT(0.707107f), -152491},
                                              {FLOAT_TO_FIXED_POINT(0.634393f), -165303},
                                              {FLOAT_TO_FIXED_POINT(0.555570f), -176524},
                                              {FLOAT_TO_FIXED_POINT(0.471397f), -186045},
                                              {FLOAT_TO_FIXED_POINT(0.382683f), -193774},
                                              {FLOAT_TO_FIXED_POINT(0.290285f), -199637},
                                              {FLOAT_TO_FIXED_POINT(0.195090f), -203577},
                                              {FLOAT_TO_FIXED_POINT(0.098017f), -205556},
                                              {FLOAT_TO_FIXED_POINT(0.000000f), -205556},
                                              {FLOAT_TO_FIXED_POINT(-0.098017f), -203577},
                                              {FLOAT_TO_FIXED_POINT(-0.195090f), -199637},
                                              {FLOAT_TO_FIXED_POINT(-0.290285f), -193774},
                                              {FLOAT_TO_FIXED_POINT(-0.382683f), -186045},
                                              {FLOAT_TO_FIXED_POINT(-0.471397f), -176524},
                                              {FLOAT_TO_FIXED_POINT(-0.555570f), -165303},
                                              {FLOAT_TO_FIXED_POINT(-0.634393f), -152491},
                                              {FLOAT_TO_FIXED_POINT(-0.707107f), -138210},
                                              {FLOAT_TO_FIXED_POINT(-0.773010f), -122597},
                                              {FLOAT_TO_FIXED_POINT(-0.831470f), -105804},
                                              {FLOAT_TO_FIXED_POINT(-0.881921f), -87992},
                                              {FLOAT_TO_FIXED_POINT(-0.923880f), -69333},
                                              {FLOAT_TO_FIXED_POINT(-0.956940f), -50006},
                                              {FLOAT_TO_FIXED_POINT(-0.980785f), -30197},
                                              {FLOAT_TO_FIXED_POINT(-0.995185f), -10098},
                                              {FLOAT_TO_FIXED_POINT(-1.000000f), 10098},
                                              {FLOAT_TO_FIXED_POINT(-0.995185f), 30197},
                                              {FLOAT_TO_FIXED_POINT(-0.980785f), 50006},
                                              {FLOAT_TO_FIXED_POINT(-0.956940f), 69333},
                                              {FLOAT_TO_FIXED_POINT(-0.923880f), 87992},
                                              {FLOAT_TO_FIXED_POINT(-0.881921f), 105804},
                                              {FLOAT_TO_FIXED_POINT(-0.831470f), 122597},
                                              {FLOAT_TO_FIXED_POINT(-0.773010f), 138210},
                                              {FLOAT_TO_FIXED_POINT(-0.707107f), 152491},
                                              {FLOAT_TO_FIXED_POINT(-0.634393f), 165303},
                                              {FLOAT_TO_FIXED_POINT(-0.555570f), 176524},
                                              {FLOAT_TO_FIXED_POINT(-0.471397f), 186045},
                                              {FLOAT_TO_FIXED_POINT(-0.382683f), 193774},
                                              {FLOAT_TO_FIXED_POINT(-0.290285f), 199637},
                                              {FLOAT_TO_FIXED_POINT(-0.195090f), 203577},
                                              {FLOAT_TO_FIXED_POINT(-0.098017f), 205556}};

const LOKUP_CACHE_TYPE(fix16_t) tanTable0[] = {{FLOAT_TO_FIXED_POINT(0.0f), 27145},
                                               {FLOAT_TO_FIXED_POINT(0.41421f), 38390},
                                               {FLOAT_TO_FIXED_POINT(1.00000f), 92681}};

const LOKUP_CACHE_TYPE(fix16_t) tanTable1[] = {{FLOAT_TO_FIXED_POINT(2.41421f), 57825},
                                               {FLOAT_TO_FIXED_POINT(3.29656f), 113428},
                                               {FLOAT_TO_FIXED_POINT(5.02734f), 335926}};

const LOKUP_CACHE_TYPE(fix16_t) tanTable2[] = {{FLOAT_TO_FIXED_POINT(10.15317f), 223051},
                                               {FLOAT_TO_FIXED_POINT(13.55667f), 445566},
                                               {FLOAT_TO_FIXED_POINT(20.35547f), 1335624}};

const LOKUP_CACHE_TYPE(fix16_t) tanTable3[] = {{FLOAT_TO_FIXED_POINT(40.73548f), 890193},
                                               {FLOAT_TO_FIXED_POINT(54.31875f), 1780251},
                                               {FLOAT_TO_FIXED_POINT(81.48324f), 5340487}};

const LOKUP_CACHE_TYPE(fix16_t) tanTable4[] = {{FLOAT_TO_FIXED_POINT(162.97262f), 3560269},
                                               {FLOAT_TO_FIXED_POINT(217.29801f), 7120505},
                                               {FLOAT_TO_FIXED_POINT(325.94830f), 21361448}};

const LOKUP_CACHE_TYPE(fix16_t) tanTable5[] = {{FLOAT_TO_FIXED_POINT(651.89814f), 14240951},
                                               {FLOAT_TO_FIXED_POINT(869.19781f), 28481894},
                                               {FLOAT_TO_FIXED_POINT(1303.79704f), 85445668},
                                               {FLOAT_TO_FIXED_POINT(2607.59446f), 365979601},
                                               {(fix16_t)2147483647, 0}};

const LOKUP_CACHE_TYPE(Angle) aTan2Table[] = {{0, 20853},
                                              {326, 20813},
                                              {651, 20732},
                                              {975, 20612},
                                              {1297, 20454},
                                              {1617, 20260},
                                              {1933, 20032},
                                              {2246, 19773},
                                              {2555, 19484},
                                              {2860, 19170},
                                              {3159, 18832},
                                              {3453, 18474},
                                              {3742, 18098},
                                              {4025, 17708},
                                              {4302, 17306},
                                              {4572, 16896},
                                              {4836, 16479},
                                              {5094, 16058},
                                              {5344, 15635},
                                              {5589, 15212},
                                              {5826, 14790},
                                              {6058, 14372},
                                              {6282, 13959},
                                              {6500, 13552},
                                              {6712, 13151},
                                              {6917, 12759},
                                              {7117, 12374},
                                              {7310, 11999},
                                              {7498, 11633},
                                              {7679, 11277},
                                              {7856, 10931},
                                              {8026, 10595},
                                              {8192, 0}};

fix16_t Trigonometry_Sin(const Angle angle)
{
    size_t index = angle >> 10;

    // Extract Multiplier
    size_t interpolationMultiplier = angle & 0x3FF;

    // Calculate interpolation with scaled 32 bit integer multiplication to reduce rounding errors
    if (sinTable[index].interpolationMultiplicand < 0)
        return sinTable[index].value -
               (fix16_t)((interpolationMultiplier * (size_t)(-sinTable[index].interpolationMultiplicand)) >> 15);

    return sinTable[index].value +
           (fix16_t)((interpolationMultiplier * (size_t)sinTable[index].interpolationMultiplicand) >> 15);
}

fix16_t Trigonometry_Cos(const Angle angle)
{
    size_t index = ((uint16_t)(angle + 16384)) >> 10;

    // Extract Multiplier
    size_t interpolationMultiplier = angle & 0x3FF;

    // Calculate interpolation with scaled 32 bit integer multiplication to reduce rounding errors
    if (sinTable[index].interpolationMultiplicand < 0)
        return sinTable[index].value -
               (fix16_t)((interpolationMultiplier * (size_t)(-sinTable[index].interpolationMultiplicand)) >> 15);

    return sinTable[index].value +
           (fix16_t)((interpolationMultiplier * (size_t)sinTable[index].interpolationMultiplicand) >> 15);
}

fix16_t Trigonometry_Tan(Angle angle)
{
    if (angle > 32767)
        angle += 32768;

    bool secondQuarter = angle > 16383;

    if (secondQuarter)
        angle = 32768 - angle;

    size_t index;
    fix16_t ret;

    if (angle >= 0x3FF0)
    {
        index = (angle - 0x3FF0) >> 2;
        size_t multiplier = angle & 0x0003;
        fix16_t interpolation = (multiplier * (size_t)tanTable5[index].interpolationMultiplicand) >> 2;
        ret = tanTable5[index].value + interpolation;
        return secondQuarter ? -ret : ret;
    }

    if (angle >= 0x3FC0)
    {
        index = (angle - 0x3FC0) >> 4;
        size_t multiplier = angle & 0x000F;
        fix16_t interpolation = (multiplier * (size_t)tanTable4[index].interpolationMultiplicand) >> 4;
        ret = tanTable4[index].value + interpolation;
        return secondQuarter ? -ret : ret;
    }

    if (angle >= 0x3F00)
    {
        index = (angle - 0x3F00) >> 6;
        size_t multiplier = angle & 0x003F;
        fix16_t interpolation = (multiplier * (size_t)tanTable3[index].interpolationMultiplicand) >> 6;
        ret = tanTable3[index].value + interpolation;
        return secondQuarter ? -ret : ret;
    }

    if (angle >= 0x3C00)
    {
        index = (angle - 0x3C00) >> 8;
        size_t multiplier = angle & 0x00FF;
        fix16_t interpolation = (multiplier * (size_t)tanTable2[index].interpolationMultiplicand) >> 8;
        ret = tanTable2[index].value + interpolation;
        return secondQuarter ? -ret : ret;
    }

    if (angle >= 0x3000)
    {
        index = (angle - 0x3000) >> 10;
        size_t multiplier = angle & 0x03FF;
        fix16_t interpolation = (multiplier * (size_t)tanTable1[index].interpolationMultiplicand) >> 10;
        ret = tanTable1[index].value + interpolation;
        return secondQuarter ? -ret : ret;
    }

    index = angle >> 12;
    size_t multiplier = angle & 0x0FFF;
    fix16_t interpolation = (multiplier * (size_t)tanTable0[index].interpolationMultiplicand) >> 12;
    ret = tanTable0[index].value + interpolation;
    return secondQuarter ? -ret : ret;
}

/**
 * Fixed point division implementation to be used in Atan2.
 * Based on libyaul and isolated here so that this
 * trigonimetric module is able to be compiled without yaul.
 * */
static fix16_t fix16_t_div(const fix16_t dividend, const fix16_t divisor)
{
    static size_t const cpu = 0xFFFFF000UL;
    static volatile uint32_t *const dvsr = (volatile uint32_t *)(cpu + 0x0F00UL);
    static volatile uint32_t *const dvdnth = (volatile uint32_t *)(cpu + 0x0F10UL);
    static volatile uint32_t *const dvdntl = (volatile uint32_t *)(cpu + 0x0F14UL);

    uint32_t dividendh;
    __asm__ volatile("swap.w %[in], %[out]\n"
                     : [out] "=&r"(dividendh)
                     : [in] "r"(dividend));
    __asm__ volatile("exts.w %[in], %[out]"
                     : [out] "=&r"(dividendh)
                     : [in] "r"(dividendh));

    *dvdnth = dividendh;
    *dvsr = divisor;
    *dvdntl = dividend << 16;

    return *dvdntl;
}

static inline fix16_t fix16_t_abs(const fix16_t value)
{
    return value < 0 ? -value : value;
}

Angle Trigonometry_Atan2(const fix16_t x, const fix16_t y)
{
    Angle result = x < 0 ? (y < 0 ? PI : -PI) : 0;

    fix16_t divResult;

    if (fix16_t_abs(x) < fix16_t_abs(y))
    {
        divResult = fix16_t_div(x, y);
        result += divResult < 0 ? -HALF_PI : HALF_PI;
    }
    else
    {
        divResult = fix16_t_div(y, x);
    }

    if (divResult < 0)
    {
        divResult = -divResult;
        size_t index = divResult >> 11;
        size_t multiplier = divResult & 0x7FF;
        uint16_t interpolation = ((multiplier * (size_t)aTan2Table[index].interpolationMultiplicand) >> 17);
        uint16_t computedValue = aTan2Table[index].value + interpolation;
        return result + computedValue;
    }

    size_t index = divResult >> 11;
    size_t multiplier = divResult & 0x7FF;
    uint16_t interpolation = ((multiplier * (size_t)aTan2Table[index].interpolationMultiplicand) >> 17);
    uint16_t computedValue = aTan2Table[index].value + interpolation;
    return result - computedValue;
}
