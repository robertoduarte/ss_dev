#pragma once

#include "Fxp.hpp"
#include "..\Utils\StringUtils.hpp"
#include <math.h>

struct Fxp3D
{
    Fxp x;
    Fxp y;
    Fxp z;

    constexpr Fxp3D() : x(), y(), z() {}

    constexpr Fxp3D(const Fxp3D &fxp3D) : x(fxp3D.x), y(fxp3D.y), z(fxp3D.z) {}

    constexpr Fxp3D(const Fxp &x, const Fxp &y, const Fxp &z) : x(x), y(y), z(z) {}

    inline Fxp3D(const char *str) { Parse(str); }

    inline size_t Parse(const char *str)
    {
        const char *tmpStr = str;
        tmpStr += x.Parse(tmpStr);

        if (StringUtils::SkipUntil(&tmpStr, ','))
            tmpStr += y.Parse(++tmpStr);

        if (StringUtils::SkipUntil(&tmpStr, ','))
            tmpStr += z.Parse(++tmpStr);

        return tmpStr - str;
    }

    constexpr Fxp3D operator-() const { return Fxp3D(-x, -y, -z); }

    constexpr Fxp3D operator+(const Fxp3D &fxp3D) const
    {
        return Fxp3D(x + fxp3D.x,
                     y + fxp3D.y,
                     z + fxp3D.z);
    }

    constexpr Fxp3D operator-(const Fxp3D &fxp3D) const
    {
        return Fxp3D(x - fxp3D.x,
                     y - fxp3D.y,
                     z - fxp3D.z);
    }

    inline Fxp Dot(const Fxp3D &fxp3D) const
    {
        return x * fxp3D.x +
               y * fxp3D.y +
               z * fxp3D.z;
    }

    inline Fxp DotV2(const Fxp3D &fxp3D) const
    {
        return fix16_vec3_dot((const fix16_vec3_t *)this, (const fix16_vec3_t *)&fxp3D);
    }

    inline Fxp3D Cross(const Fxp3D &fxp3D) const
    {
        return Fxp3D(z * fxp3D.y - y * fxp3D.z,
                     -(x * fxp3D.z - z * fxp3D.x),
                     y * fxp3D.x - x * fxp3D.y);
    }

    inline Fxp3D operator*(const Fxp &fxp) const
    {
        return Fxp3D(x * fxp,
                     y * fxp,
                     z * fxp);
    }

    inline Fxp3D operator/(const Fxp &fxp) const
    {
        return Fxp3D(x / fxp,
                     y / fxp,
                     z / fxp);
    }

    inline Fxp Length() const
    {
        return Fxp(x.Square() +
                   y.Square() +
                   z.Square());
    }

    inline Fxp3D Normalize() const
    {
        Fxp length = Length();
        if (length != 0.0F)
            return Fxp3D(length / x,
                         length / y,
                         length / z);
        else
            return Fxp3D();
    }

    static inline Fxp3D CalcNormal(const Fxp3D &vertexA, const Fxp3D &vertexB, const Fxp3D &vertexC)
    {
        return (vertexA - vertexB).Cross(vertexC - vertexB);
    }
};
