#ifndef COMPONENT_H
#define COMPONENT_H

#include <stdlib.h>
#include "ComponentList.h"

#define AS_COMPONENT(_) Component_##_,
typedef enum Component
{
    COMPONENTS(AS_COMPONENT)
} Component;

#define AS_COMPONENT_ID(_) ComponentId_##_ = (1 << Component_##_),
typedef enum ComponentId
{
    COMPONENTS(AS_COMPONENT_ID)
} ComponentId;

size_t ComponentGetSize(Component component);

ComponentId ComponentGetId(Component component);

#endif // !COMPONENT_H
