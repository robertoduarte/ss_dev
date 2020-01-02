#include "Entity.h"
#include "..\EventSystem\EventManager.h"
static short LastEntity = -1;
static short EntityCapacity = 0;

inline void Entity_Init(short entityCapacity)
{
    if (entityCapacity > 0)
    {
        EntityCapacity = entityCapacity;
    }
}

inline short Entity_AssignId()
{
    return (!(LastEntity + 1 < EntityCapacity)) ? -1 : ++LastEntity;
}

inline void Entity_FreeId(short entityId)
{
    if (entityId >= 0 && entityId < LastEntity)
    {
        EntityUpdate update = {LastEntity, entityId};
        EventManager_TriggerEvent(UpdateEntity, (unsigned)&update);
    }
    if (entityId == LastEntity)
    {

        EventManager_TriggerEvent(RemoveEntity, (unsigned)LastEntity);
    }
    LastEntity--;
}
