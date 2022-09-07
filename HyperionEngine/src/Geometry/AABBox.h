#pragma once

#include "..\Math\Fxp3D.h"

typedef enum
{
    VertexType_N,
    VertexType_P
} VertexType;

Fxp3D AABBox_GetVertex(Fxp3DPtr position, Fxp size, Fxp3DPtr normal, VertexType type);


