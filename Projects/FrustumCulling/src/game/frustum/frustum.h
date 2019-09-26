#ifndef FRUSTUM_H
#define FRUSTUM_H

#include "aabox.h"
#include "plane.h"

typedef enum
{
    R_320x224 = F16(1.42857142857142857143),
    R_320x240 = F16(1.33333333333333333333),
    R_320x256 = F16(1.25),
    R_352x224 = F16(1.57142857142857142857),
    R_352x240 = F16(1.46666666666666666667),
    R_352x256 = F16(1.375),
    R_640x224 = F16(2.85714285714285714286),
    R_640x240 = F16(2.66666666666666666667),
    R_640x256 = F16(2.5),
    R_704x224 = F16(3.14285714285714285714),
    R_704x240 = F16(2.93333333333333333333),
    R_704x256 = F16(2.75),
    R_320x448 = F16(0.71428571428571428571),
    R_320x480 = F16(0.66666666666666666667),
    R_320x512 = F16(0.625),
    R_352x448 = F16(0.78571428571428571429),
    R_352x480 = F16(0.73333333333333333333),
    R_352x512 = F16(0.6875),
    R_640x448 = F16(1.42857142857142857143),
    R_640x480 = F16(1.33333333333333333333),
    R_640x512 = F16(1.25),
    R_704x448 = F16(1.57142857142857142857),
    R_704x480 = F16(1.46666666666666666667),
    R_704x512 = F16(1.375)
} ASPECT_RATIO;

typedef enum
{
    TOP_PLANE = 0,
    BOTTOM_PLANE = 1,
    LEFT_PLANE = 2,
    RIGHT_PLANE = 3,
    NEAR_PLANE = 4,
    FAR_PLANE = 5
} FRUSTUM_PLANE;

typedef enum
{
    OUTSIDE_FRUSTUM = 0,
    INTERSECTS_FRUSTUM = 1,
    INSIDE_FRUSTUM = 2
} FRUSTUM_INTERSECTION;

typedef struct
{
    FIXED nearDistance, farDistance;
    FIXED nearWidth, nearHeight, farWidth, farHeight;
    Plane plane[6];
} Frustum;

void Frustum_init(Frustum* frustum, Sint16 fov, ASPECT_RATIO ratio, FIXED nearDistance, FIXED farDistance);

void Frustum_updateCam(Frustum* frustum);

FRUSTUM_INTERSECTION Frustum_pointInFrustum(Frustum* frustum, POINT p);

FRUSTUM_INTERSECTION Frustum_sphereInFrustum(Frustum* frustum, POINT p, FIXED radius);

FRUSTUM_INTERSECTION Frustum_boxInFrustum(Frustum* frustum, AABox* b);

#endif /* FRUSTUM_H */

