#include "ComponentManager.h"
#include <string.h>

struct ComponentManager
{
    short componentSize;
    short lastComponent;
    short entityCapacity;
    short componentCapacity;
    short *entityComponentMap;
    short *componentEntityMap;
    void *components;
};

ComponentManager *ComponentManager_New(short componentSize, short entityCapacity, short componentCapacity)
{
    ComponentManager *manager = malloc(sizeof(ComponentManager));
    manager->componentSize = componentSize;
    manager->lastComponent = -1;
    manager->entityCapacity = entityCapacity;
    manager->componentCapacity = componentCapacity;

    manager->entityComponentMap = malloc(sizeof(manager->entityComponentMap[0]) * entityCapacity);
    memset(manager->entityComponentMap, (short)-1, sizeof(manager->entityComponentMap[0]) * entityCapacity);

    manager->componentEntityMap = malloc(sizeof(manager->componentEntityMap[0]) * componentCapacity);
    memset(manager->componentEntityMap, (short)-1, sizeof(manager->componentEntityMap[0]) * componentCapacity);

    manager->components = malloc(componentSize * componentCapacity);

    return manager;
}

void ComponentManager_Delete(ComponentManager *manager)
{
    free(manager->componentEntityMap);
    free(manager->entityComponentMap);
    free(manager->components);
    free(manager);
}

short ComponentManager_AssignComponent(ComponentManager *manager, short entityId)
{
    if (!(manager->lastComponent + 1 < manager->componentCapacity))
    {
        return -1;
    }
    manager->lastComponent++;
    manager->componentEntityMap[manager->lastComponent] = entityId;
    manager->entityComponentMap[entityId] = manager->lastComponent;
    return manager->lastComponent;
}

short ComponentManager_EntityFromComponent(ComponentManager *manager, short componentId)
{
    if (componentId < 0 || componentId > manager->lastComponent)
    {
        return -1;
    }
    return manager->componentEntityMap[componentId];
}

short ComponentManager_ComponentFromEntity(ComponentManager *manager, short entityId)
{
    if (entityId < 0 || entityId >= manager->entityCapacity)
    {
        return -1;
    }
    return manager->entityComponentMap[entityId];
}

void *ComponentManager_ComponentAt(ComponentManager *manager, short componentId)
{
    if (componentId < 0 || componentId > manager->lastComponent)
    {
        return NULL;
    }
    char *componentPtr = manager->components;

    return componentPtr + (manager->componentSize * componentId);
}

void ComponentManager_RemoveComponent(ComponentManager *manager, short componentId)
{
    if (componentId < 0 || componentId > manager->lastComponent)
    {
        return;
    }

    if (!componentId == manager->lastComponent)
    {
        void *componentToRemove = ComponentManager_ComponentAt(manager, componentId);
        void *lastComponent = ComponentManager_ComponentAt(manager, manager->lastComponent);
        memcpy(componentToRemove, lastComponent, manager->componentSize);
    }
    else
    {
        short entityId = manager->componentEntityMap[componentId];
        manager->componentEntityMap[componentId] = -1;
        manager->entityComponentMap[entityId] = -1;
    }
    manager->lastComponent--;
}

void ComponentManager_ClearComponent(ComponentManager *manager, short componentId)
{
    if (componentId < 0 || componentId > manager->lastComponent)
    {
        return;
    }
    memset(ComponentManager_ComponentAt(manager, componentId), 0, manager->componentSize);
}

void ComponentManager_UpdateEntity(ComponentManager *manager, short oldId, short newId)
{
    short componentId = ComponentManager_ComponentFromEntity(manager, oldId);
    if (componentId != -1)
    {
        manager->entityComponentMap[oldId] = -1;
        manager->entityComponentMap[newId] = componentId;
        manager->componentEntityMap[componentId] = newId;
    }
}