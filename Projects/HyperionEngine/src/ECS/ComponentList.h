#ifndef COMPONENT_LIST_H
#define COMPONENT_LIST_H

#include "../Utils/XMacroUtils.h"
#include "../Math/Fxp3D.h"

#define COMPONENTS(_) \
    _(Motion)         \
    _(Health)         \
    _(Position)       \
    _(Transform)

/* Error if number of components is greater than 32. */
MACRO_ASSERT(COUNT(COMPONENTS) > 32)

typedef struct Motion
{
    Fxp3D velocity;
    Fxp3D acceleration;
} Motion;

typedef struct Health
{
    unsigned short current;
    unsigned short max;
} Health;

typedef Fxp3D Position;

typedef struct Transform
{
    Fxp3D rotation;
    Fxp3D scale;
} Transform;

#endif // !COMPONENT_LIST_H
