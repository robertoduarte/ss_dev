#ifndef ENTITY_SYSTEM_H
#define ENTITY_SYSTEM_H
#include "SL_DEF.H"
#include "ComponentTypes.h"

#define MAX_ENTITIES 100
#define UNUSED_COMPONENT -1

#define EntityId Sint16
#define ComponentId Sint16

inline void EntitySystem_Initialize();

inline EntityId EntitySystem_AssignId();

inline void EntitySystem_FreeId(EntityId entityId);

inline void EntitySystem_SetComponentId(EntityId entityId, ComponentType type, ComponentId componentId);

inline ComponentId EntitySystem_GetComponentId(EntityId entityId, ComponentType type);

#endif // !ENTITY_SYSTEM_H
