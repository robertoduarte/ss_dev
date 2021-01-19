#include "ComponentContainer.h"
#include "EntityManager.h"

#include <stdbool.h>

void *ComponentContainer_GetComponent(ComponentContainer *container, ComponentMap *map, Component Component)
{
    if (map->componentPosition[Component] < 0)
        return NULL;
    else
        return ((char *)&container->components) + map->componentPosition[Component];
}

ComponentContainerVector *ComponentContainerVector_Init(size_t archetypeSize)
{
    return Vector_Init(sizeof(EntityId) + archetypeSize, ENTITY_CAPACITY / 4);
}

ComponentContainer *ComponentContainerVector_Pointer(ComponentContainerVector *vector, int position)
{
    char *containers = vector->data;
    return (ComponentContainer *)(containers + (vector->dataSize * position));
}

void ComponentContainerVector_Push(ComponentContainerVector *vector, EntityId entityId)
{
    ComponentContainer *container = ComponentContainerVector_Pointer(vector, Vector_Push(vector));
    container->entityId = entityId;
}
