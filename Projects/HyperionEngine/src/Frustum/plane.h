#ifndef PLANE_H
#define PLANE_H

#include "vectorMath.h"

typedef struct
{
    FxVector normal;
    FxPoint point;
    FIXED d;
} Plane;

inline Plane CreatePlane(FxPoint vertex1, FxPoint vertex2, FxPoint vertex3);

inline FIXED PlaneDistance(Plane *plane, FxPoint point);

#endif /* PLANE_H */
