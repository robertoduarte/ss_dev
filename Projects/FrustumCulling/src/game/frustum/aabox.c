#include "aabox.h"
#include "fxMath.h"

void AABox_setBox(AABox* aabox, FIXED halfExtent, FIXED x, FIXED y, FIXED z)
{
    aabox->halfExtent = halfExtent;
    aabox->center[X] = x;
    aabox->center[Y] = y;
    aabox->center[Z] = z;
}

void AABox_getVertexP(AABox* aabox, VECTOR normal, FIXED result[XYZ])
{
    if (normal[X] >= 0)
        result[X] = fxAdd(aabox->center[X], aabox->halfExtent);
    else
        result[X] = fxSub(aabox->center[X], aabox->halfExtent);

    if (normal[Y] >= 0)
        result[Y] = fxAdd(aabox->center[Y], aabox->halfExtent);
    else
        result[Y] = fxSub(aabox->center[Y], aabox->halfExtent);

    if (normal[Z] >= 0)
        result[Z] = fxAdd(aabox->center[Z], aabox->halfExtent);
    else
        result[Z] = fxSub(aabox->center[Z], aabox->halfExtent);
}

void AABox_getVertexN(AABox* aabox, VECTOR normal, FIXED result[XYZ])
{
    if (normal[X] >= 0)
        result[X] = fxSub(aabox->center[X], aabox->halfExtent);
    else
        result[X] = fxAdd(aabox->center[X], aabox->halfExtent);

    if (normal[Y] >= 0)
        result[Y] = fxSub(aabox->center[Y], aabox->halfExtent);
    else
        result[Y] = fxAdd(aabox->center[Y], aabox->halfExtent);

    if (normal[Z] >= 0)
        result[Z] = fxSub(aabox->center[Z], aabox->halfExtent);
    else
        result[Z] = fxAdd(aabox->center[Z], aabox->halfExtent);
}
