#pragma once

#include "Fxp.hpp"

class Angle
{
private:
    uint16_t value;

    static constexpr uint16_t sinTable[] =
        {0x0000, 0x648, 0xC8F, 0x12D5, 0x1917, 0x1F56, 0x2590, 0x2BC4,
         0x31F1, 0x3817, 0x3E33, 0x4447, 0x4A50, 0x504D, 0x563E, 0x5C22,
         0x61F7, 0x67BD, 0x6D74, 0x7319, 0x78AD, 0x7E2E, 0x839C, 0x88F5,
         0x8E39, 0x9368, 0x987F, 0x9D7F, 0xA267, 0xA736, 0xABEB, 0xB085,
         0xB504, 0xB968, 0xBDAE, 0xC1D8, 0xC5E4, 0xC9D1, 0xCD9F, 0xD14D,
         0xD4DB, 0xD848, 0xDB94, 0xDEBE, 0xE1C5, 0xE4AA, 0xE76B, 0xEA09,
         0xEC83, 0xEED8, 0xF109, 0xF314, 0xF4FA, 0xF6BA, 0xF853, 0xF9C7,
         0xFB14, 0xFC3B, 0xFD3A, 0xFE13, 0xFEC4, 0xFF4E, 0xFFB1, 0xFFEC,
         0xFFFF};

    constexpr Fxp GetValue(const uint8_t &index) const
    {
        // Extract interpolation multiplier from second half of value (8bits)
        int32_t interpolationMultiplier = value & 0xFF;

        // Pre calculated interpolation multiplicand.
        // Since multiplicand only has 6 variations a branchless expression
        // was used instead of a table or conditional expressions.
        //  0 to 20 = 6
        // 21 to 30 = 5
        // 31 to 39 = 4
        // 41 to 46 = 3
        // 47 to 53 = 2
        // 54 to 60 = 1
        // 61 to 64 = 0
        int32_t interpolationMultiplicand =
            (index < 61) + (index < 54) + (index < 47) +
            (index < 40) + (index < 31) + (index < 21);

        int32_t interpolation = interpolationMultiplier * interpolationMultiplicand;

        // Compensation for missing bit on index 64 for 1.0 fixed point value.
        // Table has 16bit integers and cannot store value 1.0 on index 64 so:
        // (65535 + 1) = 65536 = 1.0 fixed point.
        int32_t index64compensation = index == 64;

        return sinTable[index] + index64compensation + interpolation;
    }

    constexpr Angle(const uint16_t &value) : value(value) {}

public:
    constexpr Angle() : value(0) {}

    static consteval Angle DegreesToAngle(const float &degrees)
    {
        return TurnsToAngle(degrees / 360.0F);
    }

    static constexpr Angle TurnsToAngle(const Fxp &turns)
    {
        return turns.value < 0 ? -turns.value : turns.value;
    }

    constexpr Fxp Sin() const
    {
        // Extract table index from first half of value.
        // First 8bits from 16, 256 possible values.
        uint8_t index = value >> 8;

        // Re-use first quadrant values
        if ((index & 128) == 0)
        {
            if ((index & 64) == 0)
                return GetValue(index); // 0 to 63
            else
                return GetValue(128 - index); // 64 to 127
        }
        else
        {
            if ((index & 64) == 0)
                return -GetValue(index - 128); // 128 to 191
            else
                return -GetValue(256 - index); // 192 to 255
        }
    }

    constexpr Fxp Cos() const
    {
        // Extract table index from first half of value.
        // First 8bits from 16, 256 possible values.
        uint8_t index = value >> 8;

        // Re-use sinTable first quadrant values for cosine
        if ((index & 128) == 0)
        {
            if ((index & 64) == 0)
                return GetValue(64 - index); // 0 to 63
            else
                return -GetValue(index - 64); // 64 to 127
        }
        else
        {
            if ((index & 64) == 0)
                return -GetValue(128 - index); // 128 to 191
            else
                return GetValue(index - 192); // 192 to 255
        }
    }

    inline Fxp Tan() const
    {
        Fxp::AsyncDivSet(1.0F, Cos());
        return Sin() * Fxp::AsyncDivGet();
    }

    constexpr Angle &operator+=(const Angle &a)
    {
        value += a.value;
        return *this;
    }

    constexpr Angle &operator-=(const Angle &a)
    {
        value -= a.value;
        return *this;
    }

    constexpr Angle operator+(const Angle &a) const { return Angle(*this) += a; }
    constexpr Angle operator-(const Angle &a) const { return Angle(*this) -= a; }
    constexpr bool operator>(const Angle &angle) const { return value > angle.value; }
    constexpr bool operator>=(const Angle &angle) const { return value >= angle.value; }
    constexpr bool operator<(const Angle &angle) const { return value < angle.value; }
    constexpr bool operator<=(const Angle &angle) const { return value <= angle.value; }
    constexpr bool operator==(const Angle &angle) const { return value == angle.value; }
    constexpr bool operator!=(const Angle &angle) const { return value != angle.value; }
};
