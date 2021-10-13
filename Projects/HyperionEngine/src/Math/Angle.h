#pragma once

#include "Fxp.h"

typedef unsigned short Angle;

#define DEG_TO_ANGLE_RATIO (65536.0 / 360.0)
#define DEG_TO_ANGLE(value) (Angle)(value * DEG_TO_ANGLE_RATIO)

Fxp Angle_Tan(Angle angle);

