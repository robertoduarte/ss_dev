#pragma once

#include "AABBox.h"
#include "Plane3D.h"
#include "..\Math\Angle.h"
#include <stdbool.h>

typedef enum
{
    AspectRatio_320x224 = FXP_SET(320.0 / 224.0),
    AspectRatio_320x240 = FXP_SET(320.0 / 240.0),
    AspectRatio_320x256 = FXP_SET(320.0 / 256.0),
    AspectRatio_352x224 = FXP_SET(352.0 / 224.0),
    AspectRatio_352x240 = FXP_SET(352.0 / 240.0),
    AspectRatio_352x256 = FXP_SET(352.0 / 256.0),
    AspectRatio_640x224 = FXP_SET(640.0 / 224.0),
    AspectRatio_640x240 = FXP_SET(640.0 / 240.0),
    AspectRatio_640x256 = FXP_SET(640.0 / 256.0),
    AspectRatio_704x224 = FXP_SET(704.0 / 224.0),
    AspectRatio_704x240 = FXP_SET(704.0 / 240.0),
    AspectRatio_704x256 = FXP_SET(704.0 / 256.0),
    AspectRatio_320x448 = FXP_SET(320.0 / 448.0),
    AspectRatio_320x480 = FXP_SET(320.0 / 480.0),
    AspectRatio_320x512 = FXP_SET(320.0 / 512.0),
    AspectRatio_352x448 = FXP_SET(352.0 / 448.0),
    AspectRatio_352x480 = FXP_SET(352.0 / 480.0),
    AspectRatio_352x512 = FXP_SET(352.0 / 512.0),
    AspectRatio_640x448 = FXP_SET(640.0 / 448.0),
    AspectRatio_640x480 = FXP_SET(640.0 / 480.0),
    AspectRatio_640x512 = FXP_SET(640.0 / 512.0),
    AspectRatio_704x448 = FXP_SET(704.0 / 448.0),
    AspectRatio_704x480 = FXP_SET(704.0 / 480.0),
    AspectRatio_704x512 = FXP_SET(704.0 / 512.0)
} AspectRatio;

typedef enum
{
    Plane_Near,
    Plane_Far,
    Plane_Top,
    Plane_Bottom,
    Plane_Left,
    Plane_Right,
    Plane_Count
} Plane;

typedef struct
{
    Fxp nearDistance, farDistance;
    Fxp farWidth, farHeight;
    Plane3D plane[Plane_Count];
} Frustum;

Frustum CreateFrustum(Angle fov, AspectRatio ratio, Fxp nearDistance, Fxp farDistance);

void Frustum_Update(Frustum *frustum, Fxp matrix[4][3]);

bool Frustum_PointInFrustum(Frustum *frustum, Fxp3DPtr position);

bool Frustum_SphereInFrustum(Frustum *frustum, Fxp3DPtr position, Fxp size);

bool Frustum_BoxInFrustum(Frustum *frustum, Fxp3DPtr position, Fxp size);
