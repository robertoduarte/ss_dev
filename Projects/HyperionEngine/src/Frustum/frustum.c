#include "frustum.h"

typedef enum
{
    TOP_PLANE,
    BOTTOM_PLANE,
    LEFT_PLANE,
    RIGHT_PLANE,
    NEAR_PLANE,
    FAR_PLANE
} FRUSTUM_PLANE;

Frustum CreateFrustum(Sint16 fov, ASPECT_RATIO ratio, FIXED nearDistance, FIXED farDistance)
{
    FIXED tangent = slTan(DEGtoANG(fov / 2));

    Frustum frustum;
    frustum.farDistance = farDistance;
    frustum.nearDistance = nearDistance;
    frustum.nearHeight = slMulFX(nearDistance, tangent);
    frustum.nearWidth = slMulFX(frustum.nearHeight, ratio);
    frustum.farHeight = slMulFX(farDistance, tangent);
    frustum.farWidth = slMulFX(frustum.farHeight, ratio);

    return frustum;
}

void Frustum_updateCam(Frustum *frustum)
{
    FIXED matrix[4][3];
    slGetMatrix(matrix);

    FxPoint xAxis = (FxPoint){matrix[0][X], matrix[0][Y], matrix[0][Z]};
    FxPoint yAxis = (FxPoint){matrix[1][X], matrix[1][Y], matrix[1][Z]};
    FxPoint zAxis = (FxPoint){matrix[2][X], matrix[2][Y], matrix[2][Z]};
    FxPoint position = (FxPoint){matrix[3][X], matrix[3][Y], matrix[3][Z]};

    FxVector auxiliarVector1;
    FxVector auxiliarVector2;

    // compute the center of the near and far planes
    FxVector nearCentre = fxVectorAdd(position, fxVectorMult(zAxis, frustum->nearDistance));
    FxVector farCentre = fxVectorAdd(position, fxVectorMult(zAxis, frustum->farDistance));

    // compute the 8 corners of the frustum
    auxiliarVector1 = fxVectorMult(yAxis, frustum->nearHeight);
    auxiliarVector2 = fxVectorMult(xAxis, frustum->nearWidth);

    FxVector nearTopLeft = fxVectorSub(fxVectorAdd(nearCentre, auxiliarVector1), auxiliarVector2);
    FxVector nearTopRight = fxVectorAdd(fxVectorAdd(nearCentre, auxiliarVector1), auxiliarVector2);
    FxVector nearBottomLeft = fxVectorSub(fxVectorSub(nearCentre, auxiliarVector1), auxiliarVector2);
    FxVector nearBottomRight = fxVectorAdd(fxVectorSub(nearCentre, auxiliarVector1), auxiliarVector2);

    auxiliarVector1 = fxVectorMult(yAxis, frustum->farHeight);
    auxiliarVector2 = fxVectorMult(xAxis, frustum->farWidth);

    FxVector farTopLeft = fxVectorSub(fxVectorAdd(farCentre, auxiliarVector1), auxiliarVector2);
    FxVector farTopRight = fxVectorAdd(fxVectorAdd(farCentre, auxiliarVector1), auxiliarVector2);
    FxVector farBottomLeft = fxVectorSub(fxVectorSub(farCentre, auxiliarVector1), auxiliarVector2);
    FxVector farBottomRight = fxVectorAdd(fxVectorSub(farCentre, auxiliarVector1), auxiliarVector2);

    frustum->plane[TOP_PLANE] = CreatePlane(nearTopRight, nearTopLeft, farTopLeft);
    frustum->plane[BOTTOM_PLANE] = CreatePlane(nearBottomLeft, nearBottomRight, farBottomRight);
    frustum->plane[LEFT_PLANE] = CreatePlane(nearTopLeft, nearBottomLeft, farBottomLeft);
    frustum->plane[RIGHT_PLANE] = CreatePlane(nearBottomRight, nearTopRight, farBottomRight);
    frustum->plane[NEAR_PLANE] = CreatePlane(nearTopLeft, nearTopRight, nearBottomRight);
    frustum->plane[FAR_PLANE] = CreatePlane(farTopRight, farTopLeft, farBottomLeft);
}

FRUSTUM_INTERSECTION Frustum_pointInFrustum(Frustum *frustum, FxPoint p)
{
    for (int i = 0; i < 6; i++)
    {
        if (PlaneDistance(&frustum->plane[i], p) < 0)
            return OUTSIDE_FRUSTUM;
    }
    return INSIDE_FRUSTUM;
}

FRUSTUM_INTERSECTION Frustum_sphereInFrustum(Frustum *frustum, FxPoint p, FIXED radius)
{
    int result = INSIDE_FRUSTUM;
    FIXED distance;

    for (int i = 0; i < 6; i++)
    {
        distance = PlaneDistance(&frustum->plane[i], p);
        if (distance < -radius)
            return OUTSIDE_FRUSTUM;
        else if (distance < radius)
            result = INTERSECTS_FRUSTUM;
    }
    return (result);
}

FRUSTUM_INTERSECTION Frustum_boxInFrustum(Frustum *frustum, AABox *b)
{
    for (int i = 0; i < 6; i++)
    {
        if (PlaneDistance(&frustum->plane[i], AABox_getVertexN(b, frustum->plane[i].normal)) < 0)
        {
            if (PlaneDistance(&frustum->plane[i], AABox_getVertexP(b, frustum->plane[i].normal)) > 0)
                return INTERSECTS_FRUSTUM;
            else
                return OUTSIDE_FRUSTUM;
        }
    }
    return INSIDE_FRUSTUM;
}
