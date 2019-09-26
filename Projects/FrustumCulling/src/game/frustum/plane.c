#include "plane.h"

void Plane_set3Points(Plane* plane, POINT vertex1, POINT vertex2, POINT vertex3)
{
    VECTOR auxiliarVector1;
    fxVectorNormalize(auxiliarVector1);
    fxVectorSub(auxiliarVector1, vertex1, vertex2);

    VECTOR auxiliarVector2;
    fxVectorSub(auxiliarVector2, vertex3, vertex2);
    fxVectorNormalize(auxiliarVector2);

    fxVectorCross(plane->normal, auxiliarVector1, auxiliarVector2);
    fxVectorNormalize(plane->normal);

    fx3dCopy(plane->point, vertex2);
    
    plane->d = slInnerProduct(plane->normal, plane->point);
}

FIXED Plane_distance(Plane* plane, POINT point)
{
    return fxSub(plane->d, slInnerProduct(plane->normal, point));
}
