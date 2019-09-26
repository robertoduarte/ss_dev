#ifndef fxVectorH
#define fxVectorH

#include "fxMath.h"

void fx3dSet(FIXED result[XYZ], FIXED x, FIXED y, FIXED z);

void fx3dCopy(FIXED result[XYZ], FIXED source[XYZ]);

void fxVectorAdd(VECTOR dest, VECTOR a, VECTOR b);

void fxVectorSub(VECTOR dest, VECTOR a, VECTOR b);

void fxVectorCross(VECTOR dest, VECTOR a, VECTOR b);

void fxVectorMult(VECTOR dest, VECTOR a, FIXED b);

void fxVectorDiv(VECTOR dest, VECTOR a, FIXED b);

void fxVectorNormalize(VECTOR vector);

FIXED fxVectorInnerProduct(VECTOR a, VECTOR b);

#endif /* fxVectorH */

