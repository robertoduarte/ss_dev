#ifndef FRUSTUM_H
#define FRUSTUM_H
#include <SL_DEF.H>
#include "aabox.h"
#include "plane.h"

#undef toFIXED
#define toFIXED(x) ((FIXED)(((x) >= 0) ? ((x)*65536.0 + 0.5) : ((x)*65536.0 - 0.5)))

#undef DEGtoANG
#define DEGtoANG(x) ((FIXED)(((x) >= 0) ? ((x)*65536.0 + 0.5) : ((x)*65536.0 - 0.5)) / 360)

typedef enum
{
    R_320x224 = toFIXED(1.42857142857142857143),
    R_320x240 = toFIXED(1.33333333333333333333),
    R_320x256 = toFIXED(1.25),
    R_352x224 = toFIXED(1.57142857142857142857),
    R_352x240 = toFIXED(1.46666666666666666667),
    R_352x256 = toFIXED(1.375),
    R_640x224 = toFIXED(2.85714285714285714286),
    R_640x240 = toFIXED(2.66666666666666666667),
    R_640x256 = toFIXED(2.5),
    R_704x224 = toFIXED(3.14285714285714285714),
    R_704x240 = toFIXED(2.93333333333333333333),
    R_704x256 = toFIXED(2.75),
    R_320x448 = toFIXED(0.71428571428571428571),
    R_320x480 = toFIXED(0.66666666666666666667),
    R_320x512 = toFIXED(0.625),
    R_352x448 = toFIXED(0.78571428571428571429),
    R_352x480 = toFIXED(0.73333333333333333333),
    R_352x512 = toFIXED(0.6875),
    R_640x448 = toFIXED(1.42857142857142857143),
    R_640x480 = toFIXED(1.33333333333333333333),
    R_640x512 = toFIXED(1.25),
    R_704x448 = toFIXED(1.57142857142857142857),
    R_704x480 = toFIXED(1.46666666666666666667),
    R_704x512 = toFIXED(1.375)
} ASPECT_RATIO;

typedef enum
{
    OUTSIDE_FRUSTUM,
    INTERSECTS_FRUSTUM,
    INSIDE_FRUSTUM
} FRUSTUM_INTERSECTION;

typedef struct
{
    FIXED nearDistance, farDistance;
    FIXED nearWidth, nearHeight, farWidth, farHeight;
    Plane plane[6];
} Frustum;

Frustum CreateFrustum(Sint16 fov, ASPECT_RATIO ratio, FIXED nearDistance, FIXED farDistance);

void Frustum_updateCam(Frustum *frustum);

FRUSTUM_INTERSECTION Frustum_pointInFrustum(Frustum *frustum, FxPoint p);

FRUSTUM_INTERSECTION Frustum_sphereInFrustum(Frustum *frustum, FxPoint p, FIXED radius);

FRUSTUM_INTERSECTION Frustum_boxInFrustum(Frustum *frustum, AABox *b);
#endif /* FRUSTUM_H */
