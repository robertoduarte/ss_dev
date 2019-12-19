#include "ComponentManager.h"
#include <string.h>
#include <..\..\sh-coff\include\malloc.h>
#include <SL_DEF.H>

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

ComponentManager *New_ComponentManager(short componentSize, short entityCapacity, short componentCapacity)
{
    ComponentManager *manager = malloc(sizeof(ComponentManager));

    manager->componentSize = ABS(componentSize);
    manager->entityCapacity = ABS(entityCapacity);
    manager->componentCapacity = ABS(componentCapacity);

    manager->lastComponent = -1;

    manager->entityComponentMap = malloc(sizeof(manager->entityComponentMap[0]) * manager->entityCapacity);
    manager->componentEntityMap = malloc(sizeof(manager->componentEntityMap[0]) * manager->componentCapacity);
    manager->components = malloc(manager->componentSize * manager->componentCapacity);

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
    void *componentPtr = (char *)manager->components + (manager->componentSize * manager->lastComponent);
    memset(componentPtr, 0, manager->componentSize);

    return manager->lastComponent;
}

int ComponentExists(ComponentManager *manager, short componentId)
{
    return (componentId >= 0 && componentId <= manager->lastComponent);
}

int EntityExists(ComponentManager *manager, short entityId)
{
    return (entityId >= 0 && entityId < manager->entityCapacity);
}

short ComponentManager_EntityFromComponent(ComponentManager *manager, short componentId)
{
    return (ComponentExists(manager, componentId)) ? manager->componentEntityMap[componentId] : -1;
}

short ComponentManager_ComponentFromEntity(ComponentManager *manager, short entityId)
{
    return (EntityExists(manager, entityId)) ? manager->entityComponentMap[entityId] : -1;
}

void *ComponentManager_ComponentAt(ComponentManager *manager, short componentId)
{
    return (ComponentExists(manager, componentId)) ? (char *)manager->components + (manager->componentSize * componentId) : NULL;
}

void ComponentManager_RemoveComponent(ComponentManager *manager, short componentId)
{
    if (ComponentExists(manager, componentId))
    {
        if (!componentId == manager->lastComponent)
        {
            void *componentToRemove = ComponentManager_ComponentAt(manager, componentId);
            void *lastComponent = ComponentManager_ComponentAt(manager, manager->lastComponent);
            memcpy(componentToRemove, lastComponent, manager->componentSize);

            short entityId = manager->componentEntityMap[manager->lastComponent];
            manager->componentEntityMap[componentId] = entityId;
            manager->entityComponentMap[entityId] = componentId;
        }
        manager->lastComponent--;
    }
}

void ComponentManager_RemoveComponentByEntity(ComponentManager *manager, short entityId)
{
    if (EntityExists(manager, entityId))
    {
        ComponentManager_RemoveComponent(manager, manager->entityComponentMap[entityId]);
    }
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
