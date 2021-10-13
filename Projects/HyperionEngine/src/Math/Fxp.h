#pragma once

typedef signed long Fxp;
#define FXP_SET(value) (Fxp)(value * 65536.0)
#define FXP_MAX (Fxp)0x7FFFFFFF
#define FXP_MIN (Fxp)0x80000000
#define FXP_OVERFLOW FXP_MIN

Fxp Fxp_Mult(Fxp a, Fxp b);

Fxp Fxp_Div(Fxp a, Fxp b);

Fxp Fxp_Sqrt(Fxp value);

Fxp Fxp_Max(Fxp a, Fxp b);

Fxp Fxp_Square(Fxp value);

Fxp Fxp_Abs(Fxp value);

float Fxp_ToFloat(Fxp value);
