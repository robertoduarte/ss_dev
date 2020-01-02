#ifndef fxVectorH
#define fxVectorH

#include <SL_DEF.H>

typedef struct
{
    FIXED x;
    FIXED y;
    FIXED z;
} FxPoint;

#define FxVector FxPoint

FxVector fxVectorAdd(FxVector a, FxVector b);

FxVector fxVectorSub(FxVector a, FxVector b);

FxVector fxVectorCross(FxVector a, FxVector b);

FxVector fxVectorMult(FxVector a, FIXED b);

FxVector fxVectorDiv(FxVector a, FIXED b);

FxVector fxVectorNormalize(FxVector vector);

#endif /* fxVectorH */

