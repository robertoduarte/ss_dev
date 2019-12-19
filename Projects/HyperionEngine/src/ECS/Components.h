#ifndef COMPONENTS_H
#define COMPONENTS_H
#include <SL_DEF.H>

typedef struct
{
    unsigned short current;
    unsigned short max;
} Health;

typedef struct
{
    FIXED x;
    FIXED y;
    FIXED z;
} Position;

typedef struct
{
    ANGLE rotation[XYZ];
    FIXED scale[XYZ];
} Transform;

typedef struct
{
    VECTOR velocity;
    VECTOR acceleration;
} Motion;

#endif // !COMPONENTS_H
