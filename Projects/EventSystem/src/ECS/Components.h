#ifndef COMPONENTS_H
#define COMPONENTS_H
#include "SL_DEF.H"

typedef struct
{
    unsigned short current;
    unsigned short max;
} Health;

typedef struct
{
    POINT x;
    POINT y;
    POINT z;
} Transform;

typedef struct
{
    VECTOR velocity;
    VECTOR acceleration;
} Motion;

#endif // !COMPONENTS_H
