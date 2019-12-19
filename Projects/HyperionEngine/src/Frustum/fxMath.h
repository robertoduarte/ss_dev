#ifndef FXMATH_H
#define FXMATH_H

#include <sgl.h>

static const FIXED fxMaximum = 0x7FFFFFFF; /*!< the maximum value of FIXED */
static const FIXED fxMinimum = 0x80000000; /*!< the minimum value of FIXED */
static const FIXED fxOverflow = 0x80000000;

/* Macro for defining FIXED constant values.
   The functions above can't be used from e.g. global variable initializers,
   and their names are quite long also. This macro is useful for constants
   springled alongside code, e.g. F16(1.234).

   Note that the argument is evaluated multiple times, and also otherwise
   you should only use this for constant values.
 */
#define F16(x) ((FIXED)(((x) >= 0) ? ((x) * 65536.0 + 0.5) : ((x) * 65536.0 - 0.5)))
#define DEGtoANG16(x) ((FIXED)(((x) >= 0) ? ((x) * 65536.0 + 0.5) : ((x) * 65536.0 - 0.5)) / 360)

FIXED fxAdd(FIXED a, FIXED b);

FIXED fxSub(FIXED a, FIXED b);

FIXED fxMult(FIXED a, FIXED b);

FIXED fxDiv(FIXED a, FIXED b);

FIXED fxScale(FIXED value, char scale);

FIXED fxSqrt(FIXED value);

#endif /* FXMATH_H */

