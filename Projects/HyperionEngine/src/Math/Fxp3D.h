#pragma once

#include "Fxp.h"

typedef struct
{
    Fxp x;
    Fxp y;
    Fxp z;
} Fxp3D;

typedef Fxp3D *Fxp3DPtr;

Fxp3D Fxp3D_Add(Fxp3DPtr a, Fxp3DPtr b);

Fxp3D Fxp3D_Sub(Fxp3DPtr a, Fxp3DPtr b);

Fxp Fxp3D_Dot(Fxp3DPtr a, Fxp3DPtr b);

Fxp3D Fxp3D_Cross(Fxp3DPtr a, Fxp3DPtr b);

Fxp3D Fxp3D_Mult(Fxp3DPtr a, Fxp b);

Fxp3D Fxp3D_Div(Fxp3DPtr a, Fxp b);

Fxp Fxp3D_Length(Fxp3DPtr vector);

Fxp3D Fxp3D_Normalize(Fxp3DPtr vector);

Fxp3D Fxp3D_CalcNormal(Fxp3DPtr vertex1, Fxp3DPtr vertex2, Fxp3DPtr vertex3);

Fxp3D Fxp3D_Invert(Fxp3DPtr vector);

