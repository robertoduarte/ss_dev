#ifndef HEALTH_MANAGER_H
#define HEALTH_MANAGER_H
#include "ComponentManagerTemplate.h"

typedef struct
{
    Sint16 entityId;
    Uint16 currentHealth;
    Uint16 maxHealth;
} Health;

COMPONENT_MANAGER_HEADER(Health)

typedef struct
{
    Sint16 entityId;
    POINT x;
    POINT y;
    POINT z;
} Transform;

COMPONENT_MANAGER_HEADER(Transform)

typedef struct
{
    Sint16 entityId;
    VECTOR velocity;
    VECTOR acceleration;
} Motion;

COMPONENT_MANAGER_HEADER(Motion)

#endif // !COMPONENT_MANAGERS_H
