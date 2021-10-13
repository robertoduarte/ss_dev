#include "Fxp3D.h"

Fxp3D Fxp3D_Add(Fxp3DPtr a, Fxp3DPtr b)
{
    return (Fxp3D){a->x + b->x,
                   a->y + b->y,
                   a->z + b->z};
}

Fxp3D Fxp3D_Sub(Fxp3DPtr a, Fxp3DPtr b)
{
    return (Fxp3D){a->x - b->x,
                   a->y - b->y,
                   a->z - b->z};
}

Fxp Fxp3D_Dot(Fxp3DPtr a, Fxp3DPtr b)
{
    return Fxp_Mult(a->x, b->x) +
           Fxp_Mult(a->y, b->y) +
           Fxp_Mult(a->z, b->z);
}

Fxp3D Fxp3D_Cross(Fxp3DPtr a, Fxp3DPtr b)
{
    return (Fxp3D){(Fxp_Mult(a->z, b->y) - Fxp_Mult(a->y, b->z)),
                   -(Fxp_Mult(a->x, b->z) - Fxp_Mult(a->z, b->x)),
                   (Fxp_Mult(a->y, b->x) - Fxp_Mult(a->x, b->y))};
}

Fxp3D Fxp3D_Mult(Fxp3DPtr a, Fxp b)
{
    return (Fxp3D){Fxp_Mult(a->x, b),
                   Fxp_Mult(a->y, b),
                   Fxp_Mult(a->z, b)};
}

Fxp3D Fxp3D_Div(Fxp3DPtr a, Fxp b)
{
    return (Fxp3D){Fxp_Div(a->x, b),
                   Fxp_Div(a->y, b),
                   Fxp_Div(a->z, b)};
}

Fxp Fxp3D_Length(Fxp3DPtr vector)
{
    return Fxp_Sqrt(Fxp_Square(vector->x) +
                    Fxp_Square(vector->y) +
                    Fxp_Square(vector->z));
}

Fxp3D Fxp3D_Normalize(Fxp3DPtr vector)
{
    Fxp length = Fxp3D_Length(vector);
    if (length != 0)
        return (Fxp3D){Fxp_Div(length, vector->x),
                       Fxp_Div(length, vector->y),
                       Fxp_Div(length, vector->z)};
    else
        return (Fxp3D){0, 0, 0};
}

Fxp3D Fxp3D_CalcNormal(Fxp3DPtr vertex1, Fxp3DPtr vertex2, Fxp3DPtr vertex3)
{
    Fxp3D vertex1_2sub = Fxp3D_Sub(vertex1, vertex2);
    Fxp3D vertex3_2sub = Fxp3D_Sub(vertex3, vertex2);
    Fxp3D crossProduct = Fxp3D_Cross(&vertex1_2sub, &vertex3_2sub);
    return Fxp3D_Normalize(&crossProduct);
}

Fxp3D Fxp3D_Invert(Fxp3DPtr vector)
{
    return (Fxp3D){-vector->x,
                   -vector->y,
                   -vector->z};
}
