#include "frustum.h"

#include "AABBox.h"

Frustum CreateFrustum(Fxp verticalFov, Fxp ratio, Fxp nearDistance, Fxp farDistance)
{
    Frustum frustum;
    frustum.farHeight = Fxp_Tan(verticalFov);
    frustum.farWidth = Fxp_Mult(frustum.farHeight, ratio);
    frustum.nearDistance = nearDistance;
    frustum.farDistance = farDistance;
    return frustum;
}

void Frustum_Update(Frustum *frustum, Fxp matrix[4][3])
{
    Fxp3DPtr xAxis = (Fxp3DPtr)&matrix[0][0];
    Fxp3DPtr yAxis = (Fxp3DPtr)&matrix[1][0];
    Fxp3DPtr zAxis = (Fxp3DPtr)&matrix[2][0];
    Fxp3DPtr position = (Fxp3DPtr)&matrix[3][0];
    
    Fxp3D farCentre = Fxp3D_Add(position, zAxis);
    Fxp3D farHalfHeight = Fxp3D_Mult(yAxis, frustum->farHeight);
    Fxp3D farHalfWidth = Fxp3D_Mult(xAxis, frustum->farWidth);

    Fxp3D farTop = Fxp3D_Add(&farCentre, &farHalfHeight);
    Fxp3D farTopLeft = Fxp3D_Sub(&farTop, &farHalfWidth);
    Fxp3D farTopRight = Fxp3D_Add(&farTop, &farHalfWidth);

    Fxp3D farBottom = Fxp3D_Sub(&farCentre, &farHalfHeight);
    Fxp3D farBottomRight = Fxp3D_Add(&farBottom, &farHalfWidth);
    Fxp3D farBottomLeft = Fxp3D_Sub(&farBottom, &farHalfWidth);

    Fxp3D nearCentreDistance = Fxp3D_Mult(zAxis, frustum->nearDistance);
    Fxp3D nearPlanePosition = Fxp3D_Add(position, &nearCentreDistance);
    Fxp3D invertedZAxis = Fxp3D_Invert(zAxis);
    frustum->plane[Plane_Near] = Plane3D_CreateFromPosition(&invertedZAxis, &nearPlanePosition);

    Fxp3D farCentreDistance = Fxp3D_Mult(zAxis, frustum->farDistance);
    Fxp3D farPlanePosition = Fxp3D_Add(position, &farCentreDistance);
    frustum->plane[Plane_Far] = Plane3D_CreateFromPosition(zAxis, &farPlanePosition);

    frustum->plane[Plane_Top] = Plane3D_CreateFromVertices(&farTopRight, position, &farTopLeft);
    frustum->plane[Plane_Bottom] = Plane3D_CreateFromVertices(&farBottomLeft, position, &farBottomRight);
    frustum->plane[Plane_Left] = Plane3D_CreateFromVertices(&farTopLeft, position, &farBottomLeft);
    frustum->plane[Plane_Right] = Plane3D_CreateFromVertices(&farBottomRight, position, &farTopRight);
}

bool Frustum_PointInFrustum(Frustum *frustum, Fxp3DPtr position)
{
    for (unsigned int i = 0; i < Plane_Count; i++)
    {
        if (Plane3D_Distance(&frustum->plane[i], position) < 0)
            return false;
    }
    return true;
}

bool Frustum_SphereInFrustum(Frustum *frustum, Fxp3DPtr position, Fxp size)
{
    Fxp radius = Fxp_Div(size, FXP_SET(2));
    for (unsigned int i = 0; i < Plane_Count; i++)
    {
        if (Plane3D_Distance(&frustum->plane[i], position) < -radius)
            return false;
    }
    return true;
}

bool Frustum_BoxInFrustum(Frustum *frustum, Fxp3DPtr position, Fxp size)
{
    Fxp3D vertexN;
    for (unsigned int i = 0; i < Plane_Count; i++)
    {
        vertexN = AABBox_GetVertex(position, size, &frustum->plane[i].normal, VertexType_N);
        if (Plane3D_Distance(&frustum->plane[i], &vertexN) < 0)
            return false;
    }
    return true;
}
