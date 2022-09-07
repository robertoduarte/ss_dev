#pragma once

#include "..\Math\Fxp3D.h"

typedef struct
{
    Fxp3D normal;
    Fxp d;
} Plane3D;

Plane3D Plane3D_CreateFromVertices(Fxp3DPtr vertex1, Fxp3DPtr vertex2, Fxp3DPtr vertex3);

Plane3D Plane3D_CreateFromPosition(Fxp3DPtr normal, Fxp3DPtr position);

Fxp Plane3D_Distance(Plane3D *plane, Fxp3DPtr point);

