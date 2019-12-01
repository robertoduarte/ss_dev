#include "EntitySystem.h"

#include "../Utils/mem_mgr.h"

typedef enum
{
    ACTIVE,
    INACTIVE
} EntityStatus;

typedef struct
{
    EntityStatus status;
    ComponentId componentId[COMPONENT_COUNT];
} Entity;

static Entity *entityArray = NULL;

inline void EntitySystem_Initialize()
{
    if (!entityArray)
    {
        entityArray = Memmalloc(sizeof(Entity) * MAX_ENTITIES);
    }
    Sint32 entityId;
    for (entityId = 0; entityId < MAX_ENTITIES; entityId++)
    {
        entityArray[entityId].status = INACTIVE;
    }
}

inline EntityId EntitySystem_AssignId()
{
    Sint32 entityId;

    for (entityId = 0; entityId < MAX_ENTITIES; entityId++)
    {
        if (entityArray[entityId].status == INACTIVE)
        {
            entityArray[entityId].status = ACTIVE;

            Sint32 componentType;
            for (componentType = 0; componentType < COMPONENT_COUNT; componentType++)
            {
                entityArray[entityId].componentId[componentType] = UNUSED_COMPONENT;
            }
            return entityId;
        }
    }
    return -1;
}

inline void EntitySystem_FreeId(EntityId entityId)
{
    entityArray[entityId].status = INACTIVE;
}

inline void EntitySystem_SetComponentId(EntityId entityId, ComponentType type, ComponentId componentId)
{
    entityArray[entityId].componentId[type] = componentId;
}

inline ComponentId EntitySystem_GetComponentId(EntityId entityId, ComponentType type)
{
    return entityArray[entityId].componentId[type];
}
