#include "Fxp.h"

#include <yaul.h>

Fxp Fxp_Mult(Fxp a, Fxp b)
{
    return fix16_mul(a, b);
}

Fxp Fxp_Div(Fxp a, Fxp b)
{
    cpu_divu_fix16_set(a, b);
    return cpu_divu_quotient_get();
}

void Fxp_AsyncDivSet(Fxp a, Fxp b)
{
    cpu_divu_fix16_set(a, b);
}

Fxp Fxp_AsyncDivGet()
{
    return cpu_divu_quotient_get();
}

Fxp Fxp_Sqrt(Fxp value)
{
    return fix16_sqrt(value);
}

Fxp Fxp_Max(Fxp a, Fxp b)
{
    return a > b ? a : b;
}

Fxp Fxp_Square(Fxp value)
{
    return fix16_mul(value, value);
}

Fxp Fxp_Abs(Fxp value)
{
    return fix16_abs(value);
}

float Fxp_ToFloat(Fxp value)
{
    return value / 65536.0;
}

Fxp Fxp_Tan(Fxp value)
{
    return fix16_tan(value);
}