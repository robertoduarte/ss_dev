#pragma once

#include "Types.h"
#include "../Utils/MacroUtils.h"

#define TYPES(_, ...)          \
    _(Motion, ##__VA_ARGS__)   \
    _(Health, ##__VA_ARGS__)   \
    _(Position, ##__VA_ARGS__) \
    _(Transform, ##__VA_ARGS__)

#define AS_COMPONENT(type) C_##type
#define AS_ID(type) CId_##type

#define AS_COMPONENT_ENUM_ELEMENT(type) AS_COMPONENT(type),
typedef enum Component
{
    TYPES(AS_COMPONENT_ENUM_ELEMENT)
} Component;

#define AS_COMPONENT_ID_ENUM_ELEMENT(type) AS_ID(type) = (1 << AS_COMPONENT(type)),
typedef enum ComponentId
{
    TYPES(AS_COMPONENT_ID_ENUM_ELEMENT)
} ComponentId;
