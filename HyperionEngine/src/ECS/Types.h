#pragma once

#include "../Math/Fxp3D.h"

typedef struct Motion
{
    Fxp3D velocity;
    Fxp3D acceleration;
} Motion;

typedef struct Health
{
    unsigned short current;
    unsigned short max;
} Health;

typedef Fxp3D Position;

typedef struct Transform
{
    Fxp3D rotation;
    Fxp3D scale;
} Transform;
