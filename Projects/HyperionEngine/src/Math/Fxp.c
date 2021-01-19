#include "Fxp.h"

#include <SL_DEF.H>
#include <stdlib.h>

Fxp Fxp_Mult(Fxp a, Fxp b)
{
    return slMulFX(a, b);
}

Fxp Fxp_Div(Fxp a, Fxp b)
{
    return slDivFX(a, b);
}

Fxp Fxp_Sqrt(Fxp value)
{
    return slSquartFX(value);
}

Fxp Fxp_Max(Fxp a, Fxp b)
{
    return a > b ? a : b;
}

Fxp Fxp_Square(Fxp value)
{
    return Fxp_Mult(value, value);
}

Fxp Fxp_Abs(Fxp value)
{
    return abs(value);
}

float Fxp_ToFloat(Fxp value)
{
    return value / 65536.0;
}
