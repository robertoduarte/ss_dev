#include "EntityManager.h"
#include "ComponentTypes.h"

#include "SL_DEF.H"

static Sint32 LastEntityID = -1;

#define MAX_ENTITIES 10

static Sint32 EntityComponentIDMap[MAX_ENTITIES][COMPONENT_COUNT];

void EntityManager_CopyEntity(Sint32 sourceEntityID, Sint32 destinationEntityID)
{
    Sint32 index = 0;
    for (index = 0; index < COMPONENT_COUNT; index++)
    {
        EntityComponentIDMap[destinationEntityID][index] = EntityComponentIDMap[sourceEntityID][index];
    }
}

void EntityManager_ClearEntity(Sint32 entityID)
{

    Sint32 index = 0;
    for (index = 0; index < COMPONENT_COUNT; index++)
    {
        EntityComponentIDMap[entityID][index] = UNUSED_COMPONENT_ID;
    }
}

Sint32 EntityManager_AddEntity()
{
    LastEntityID++;
    EntityManager_ClearEntity(LastEntityID);
    return LastEntityID;
}

void EntityManager_SetComponent(Sint32 entityID, ComponentType componentType, Sint32 componentID)
{
    EntityComponentIDMap[entityID][componentType] = componentID;
}

void EntityManager_RemoveEntity(Sint32 entityID)
{
    if (LastEntityID != 1)
    {
        EntityManager_CopyEntity(LastEntityID, entityID);
    }
    LastEntityID--;
}
