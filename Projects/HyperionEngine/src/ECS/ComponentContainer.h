#ifndef COMPONENT_CONTAINER_H
#define COMPONENT_CONTAINER_H

#include "ComponentMap.h"
#include "EntityId.h"
#include "../Utils/Vector.h"

typedef struct ComponentContainer
{
    EntityId entityId;
    void *components;
} ComponentContainer;

typedef Vector ComponentContainerVector;

void *ComponentContainer_GetComponent(ComponentContainer *container, ComponentMap *map, Component Component);

ComponentContainerVector *ComponentContainerVector_Init(size_t archetypeSize);

ComponentContainer *ComponentContainerVector_Pointer(ComponentContainerVector *vector, int position);

void ComponentContainerVector_Push(ComponentContainerVector *vector, EntityId entityId);

#endif // !COMPONENT_CONTAINER_H
