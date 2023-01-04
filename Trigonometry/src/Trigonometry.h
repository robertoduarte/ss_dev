#pragma once

#include <stdint.h>

/**
 * Unsigned Angle representation
 * 65536 possible values (0 to 65535)
 * Full turn cannot be represented however this is fine because:
 * Value 0 (0 degrees or 0 radians) is also the same as 2PI radians or 360 degrees.
 * */
typedef uint16_t Angle;

// Fixed point representation as signed 32 bit integer.
typedef int32_t fix16_t;

#define DEGREES_TO_ANGLE(degrees) (Angle)((degrees / 360.0F) * 65536.0F)
#define PI (Angle)(65536 / 2)
#define HALF_PI (Angle)(PI / 2)

fix16_t Trigonometry_Sin(const Angle angle);

fix16_t Trigonometry_Cos(const Angle angle);

fix16_t Trigonometry_Tan(Angle angle);

Angle Trigonometry_Atan2(const fix16_t x, const fix16_t y);
