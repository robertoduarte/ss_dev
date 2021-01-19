#ifndef COMPONENT_MAP_H
#define COMPONENT_MAP_H

#include "ArchetypeId.h"
#include "Component.h"

typedef struct ComponentMap
{
    int componentPosition[COUNT(COMPONENTS)];
} ComponentMap;

ComponentMap BuildComponentMap(ArchetypeId archetypeId);

#endif // !COMPONENT_MAP_H
