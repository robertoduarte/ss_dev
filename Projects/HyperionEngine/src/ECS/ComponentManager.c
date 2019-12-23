#include "ComponentManager.h"
#include <string.h>
#include <..\..\sh-coff\include\malloc.h>

struct ComponentManager
{
    short componentSize;
    short lastComponent;
    short entityCapacity;
    short componentCapacity;
    short *componentFromEntity;
    short *entityFromComponent;
    void *components;
};

ComponentManager *New_ComponentManager(short componentSize, short entityCapacity, short componentCapacity)
{
    ComponentManager *manager = malloc(sizeof(ComponentManager));

    manager->componentSize = ABS(componentSize);
    manager->entityCapacity = ABS(entityCapacity);
    manager->componentCapacity = ABS(componentCapacity);

    manager->lastComponent = -1;

    manager->components = malloc(manager->componentSize * manager->componentCapacity);
    manager->componentFromEntity = malloc(sizeof(manager->componentFromEntity[0]) * manager->entityCapacity);
    manager->entityFromComponent = malloc(sizeof(manager->entityFromComponent[0]) * manager->componentCapacity);
    for (int i = 0; i < manager->entityCapacity; i++)
    {
        manager->entityFromComponent[i] = -1;
    }

    return manager;
}

void ComponentManager_Delete(ComponentManager *manager)
{
    free(manager->entityFromComponent);
    free(manager->componentFromEntity);
    free(manager->components);
    free(manager);
}

void *ComponentAt(ComponentManager *manager, short componentPosition)
{
    return (char *)manager->components + (manager->componentSize * componentPosition);
}

inline Bool WithinRange(ComponentManager *manager, short entityId)
{
    return (entityId >= 0 && entityId < manager->entityCapacity);
}

inline Bool EntityUsed(ComponentManager *manager, short entityId)
{
    return (manager->componentFromEntity[entityId] != -1);
}

int ComponentManager_CheckEntity(ComponentManager *manager, short entityId)
{
    return WithinRange(manager, entityId) && EntityUsed(manager, entityId);
}

#ifndef CM_UNSAFE
#define ENTITY_CHECK(manager, entityId) ComponentManager_CheckEntity(manager, entityId)
#else
#define ENTITY_CHECK(manager, entityId) 1
#endif // !CM_UNSAFE

Bool ComponentManager_CreateComponent(ComponentManager *manager, short entityId)
{
    if (manager->lastComponent + 1 < manager->componentCapacity)
    {
        manager->lastComponent++;
        manager->entityFromComponent[manager->lastComponent] = entityId;
        manager->componentFromEntity[entityId] = manager->lastComponent;
        memset(ComponentAt(manager, manager->lastComponent), 0, manager->componentSize);
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

void *ComponentManager_GetComponent(ComponentManager *manager, short entityId)
{
    return (ENTITY_CHECK(manager, entityId)) ? ComponentAt(manager, manager->componentFromEntity[entityId]) : NULL;
}

void ComponentManager_RemoveComponent(ComponentManager *manager, short entityId)
{
    if (ENTITY_CHECK(manager, entityId))
    {
        short componentPosition = manager->componentFromEntity[entityId];
        if (!componentPosition == manager->lastComponent)
        {
            void *componentToRemove = ComponentAt(manager, componentPosition);
            void *lastComponent = ComponentAt(manager, manager->lastComponent);
            memcpy(componentToRemove, lastComponent, manager->componentSize);

            short entityId = manager->entityFromComponent[manager->lastComponent];
            manager->entityFromComponent[componentPosition] = entityId;
            manager->componentFromEntity[entityId] = componentPosition;
        }
        manager->lastComponent--;
    }
}

void ComponentManager_UpdateEntityId(ComponentManager *manager, short oldId, short newId)
{
    if (ENTITY_CHECK(manager, oldId))
    {
        short componentPosition = manager->componentFromEntity[oldId];
        manager->componentFromEntity[oldId] = -1;
        manager->componentFromEntity[newId] = componentPosition;
        manager->entityFromComponent[componentPosition] = newId;
    }
}

short ComponentManager_First(ComponentManager *manager)
{
    return manager->entityFromComponent[0];
}

short ComponentManager_Next(ComponentManager *manager, short entityId)
{
    if (ENTITY_CHECK(manager, entityId))
    {
        short next = manager->componentFromEntity[entityId] + 1;
        if (next < manager->componentCapacity)
            return manager->entityFromComponent[next];
        else
            return -1;
    }
    else
    {
        return -1;
    }
}

Bool ComponentManager_Done(ComponentManager *manager, short entityId)
{
    if (ENTITY_CHECK(manager, entityId) && manager->componentFromEntity[entityId] <= manager->lastComponent)
        return FALSE;
    else
        return TRUE;
}
