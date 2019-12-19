#ifndef PLANE_H
#define PLANE_H

#include "vectorMath.h"

typedef struct
{
    VECTOR normal;
    POINT point;
    FIXED d;
} Plane;

void Plane_set3Points(Plane* plane, POINT vertex1, POINT vertex2, POINT vertex3);

FIXED Plane_distance(Plane* plane, POINT point);

#endif /* PLANE_H */

