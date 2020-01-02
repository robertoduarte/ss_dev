#include "plane.h"

inline Plane CreatePlane(FxPoint vertex1, FxPoint vertex2, FxPoint vertex3)
{
    FxVector normal = fxVectorNormalize(
        fxVectorCross(fxVectorNormalize(fxVectorSub(vertex1, vertex2)),
                      fxVectorNormalize(fxVectorSub(vertex3, vertex2))));
    
    return (Plane){
        normal,
        vertex2,
        slInnerProduct((FIXED *)&normal, (FIXED *)&vertex2)};
}

inline FIXED PlaneDistance(Plane *plane, FxPoint point)
{
    return plane->d - slInnerProduct((FIXED *)&plane->normal, (FIXED *)&point);
}
