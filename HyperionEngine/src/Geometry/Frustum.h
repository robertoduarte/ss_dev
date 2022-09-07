#pragma once

#include "Plane3D.h"
#include <stdbool.h>

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

Frustum CreateFrustum(Fxp verticalFov, Fxp ratio, Fxp nearDistance, Fxp farDistance);

void Frustum_Update(Frustum *frustum, Fxp matrix[4][3]);

bool Frustum_PointInFrustum(Frustum *frustum, Fxp3DPtr position);

bool Frustum_SphereInFrustum(Frustum *frustum, Fxp3DPtr position, Fxp size);

bool Frustum_BoxInFrustum(Frustum *frustum, Fxp3DPtr position, Fxp size);
