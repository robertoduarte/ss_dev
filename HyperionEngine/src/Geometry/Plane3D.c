#include "Plane3D.h"

Plane3D Plane3D_CreateFromPosition(Fxp3DPtr normal, Fxp3DPtr position)
{
    return (Plane3D){*normal, Fxp3D_Dot(normal, position)};
}

Plane3D Plane3D_CreateFromVertices(Fxp3DPtr vertex1, Fxp3DPtr vertex2, Fxp3DPtr vertex3)
{
    Fxp3D normal = Fxp3D_CalcNormal(vertex1, vertex2, vertex3);
    return Plane3D_CreateFromPosition(&normal, vertex2);
}

Fxp Plane3D_Distance(Plane3D *plane, Fxp3DPtr point)
{
    return (plane->d - Fxp3D_Dot(&plane->normal, point));
}
