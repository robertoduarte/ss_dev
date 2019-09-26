#include "frustum.h"

void Frustum_init(Frustum* frustum, Sint16 fov, ASPECT_RATIO ratio, FIXED nearDistance, FIXED farDistance)
{
    FIXED tangent = slTan(DEGtoANG(fov / 2));

    frustum->farDistance = farDistance;
    frustum->nearDistance = nearDistance;
    frustum->nearHeight = fxMult(nearDistance, tangent);
    frustum->nearWidth = fxMult(frustum->nearHeight, ratio);
    frustum->farHeight = fxMult(farDistance, tangent);
    frustum->farWidth = fxMult(frustum->farHeight, ratio);
}

void Frustum_updateCam(Frustum* frustum)
{
    FIXED matrix[4][3];
    slGetMatrix(matrix);

    FIXED* xAxis = matrix[0];
    FIXED* yAxis = matrix[1];
    FIXED* zAxis = matrix[2];
    FIXED* position = matrix[3];

    VECTOR nearTopLeft;
    VECTOR nearTopRight;
    VECTOR nearBottomLeft;
    VECTOR nearBottomRight;
    VECTOR farTopLeft;
    VECTOR farTopRight;
    VECTOR farBottomLeft;
    VECTOR farBottomRight;
    VECTOR nearCentre;
    VECTOR farCentre;

    VECTOR auxiliarVector1;
    VECTOR auxiliarVector2;
    VECTOR auxiliarVector3;

    // compute the center of the near and far planes
    fxVectorMult(auxiliarVector1, zAxis, frustum->nearDistance);
    fxVectorAdd(nearCentre, position, auxiliarVector1);

    fxVectorMult(auxiliarVector1, zAxis, frustum->farDistance);
    fxVectorAdd(farCentre, position, auxiliarVector1);

    // compute the 8 corners of the frustum
    fxVectorMult(auxiliarVector1, yAxis, frustum->nearHeight);
    fxVectorMult(auxiliarVector2, xAxis, frustum->nearWidth);

    fxVectorAdd(auxiliarVector3, nearCentre, auxiliarVector1);
    fxVectorSub(nearTopLeft, auxiliarVector3, auxiliarVector2);

    fxVectorAdd(auxiliarVector3, nearCentre, auxiliarVector1);
    fxVectorAdd(nearTopRight, auxiliarVector3, auxiliarVector2);

    fxVectorSub(auxiliarVector3, nearCentre, auxiliarVector1);
    fxVectorSub(nearBottomLeft, auxiliarVector3, auxiliarVector2);

    fxVectorSub(auxiliarVector3, nearCentre, auxiliarVector1);
    fxVectorAdd(nearBottomRight, auxiliarVector3, auxiliarVector2);

    fxVectorMult(auxiliarVector1, yAxis, frustum->farHeight);
    fxVectorMult(auxiliarVector2, xAxis, frustum->farWidth);

    fxVectorAdd(auxiliarVector3, farCentre, auxiliarVector1);
    fxVectorSub(farTopLeft, auxiliarVector3, auxiliarVector2);

    fxVectorAdd(auxiliarVector3, farCentre, auxiliarVector1);
    fxVectorAdd(farTopRight, auxiliarVector3, auxiliarVector2);

    fxVectorSub(auxiliarVector3, farCentre, auxiliarVector1);
    fxVectorSub(farBottomLeft, auxiliarVector3, auxiliarVector2);

    fxVectorSub(auxiliarVector3, farCentre, auxiliarVector1);
    fxVectorAdd(farBottomRight, auxiliarVector3, auxiliarVector2);

    Plane_set3Points(&frustum->plane[TOP_PLANE], nearTopRight, nearTopLeft, farTopLeft);
    Plane_set3Points(&frustum->plane[BOTTOM_PLANE], nearBottomLeft, nearBottomRight, farBottomRight);
    Plane_set3Points(&frustum->plane[LEFT_PLANE], nearTopLeft, nearBottomLeft, farBottomLeft);
    Plane_set3Points(&frustum->plane[RIGHT_PLANE], nearBottomRight, nearTopRight, farBottomRight);
    Plane_set3Points(&frustum->plane[NEAR_PLANE], nearTopLeft, nearTopRight, nearBottomRight);
    Plane_set3Points(&frustum->plane[FAR_PLANE], farTopRight, farTopLeft, farBottomLeft);
}

FRUSTUM_INTERSECTION Frustum_pointInFrustum(Frustum* frustum, POINT p)
{
    for (int i = 0; i < 6; i++)
    {
        if (Plane_distance(&frustum->plane[i], p) < 0)
            return OUTSIDE_FRUSTUM;
    }
    return INSIDE_FRUSTUM;
}

FRUSTUM_INTERSECTION Frustum_sphereInFrustum(Frustum* frustum, POINT p, FIXED radius)
{
    int result = INSIDE_FRUSTUM;
    FIXED distance;

    for (int i = 0; i < 6; i++)
    {
        distance = Plane_distance(&frustum->plane[i], p);
        if (distance < -radius)
            return OUTSIDE_FRUSTUM;
        else if (distance < radius)
            result = INTERSECTS_FRUSTUM;
    }
    return (result);
}

FRUSTUM_INTERSECTION Frustum_boxInFrustum(Frustum* frustum, AABox* b)
{
    VECTOR vertex;
    for (int i = 0; i < 6; i++)
    {
        AABox_getVertexP(b, frustum->plane[i].normal, vertex);
        if (Plane_distance(&frustum->plane[i], vertex) < 0)
        {
            AABox_getVertexN(b, frustum->plane[i].normal, vertex);
            if (Plane_distance(&frustum->plane[i], vertex) > 0)
                return INTERSECTS_FRUSTUM;
            else
                return OUTSIDE_FRUSTUM;
        }
    }
    return INSIDE_FRUSTUM;
}
