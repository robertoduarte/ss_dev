#ifndef COMPONENT_TYPES_H
#define COMPONENT_TYPES_H
#include "SL_DEF.H"

typedef struct
{
    Sint16 entityId;
    Uint16 currentHealth;
    Uint16 maxHealth;
} Health;

typedef struct
{
    Sint16 entityId;
    POINT x;
    POINT y;
    POINT z;
} Transform;

typedef struct
{
    Sint16 entityId;
    VECTOR velocity;
    VECTOR acceleration;
} Motion;

typedef enum
{
    HEALTH_COMPONENT,
    TRANSFORM_COMPONENT,
    MOTION_COMPONENT,
    COMPONENT_COUNT
} ComponentType;

typedef struct
{
    union {
        Health health;
        Transform transform;
        Motion motion;
    };
} Component;

//static const Uint8 ComponentTypeSizes[COMPONENT_COUNT] = {sizeof(Health), sizeof(Transform), sizeof(Motion)};

#endif // !COMPONENT_TYPES_H
