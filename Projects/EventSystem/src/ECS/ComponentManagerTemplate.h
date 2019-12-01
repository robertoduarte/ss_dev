#ifndef COMPONENT_MANAGER_TEMPLATE_H
#define COMPONENT_MANAGER_TEMPLATE_H

#include "../Utils/mem_mgr.h"
#include "EntitySystem.h"

#define COMPONENT_MANAGER_HEADER(TYPE)                     \
    void TYPE##Manager_Initialize(Sint16 max);             \
                                                           \
    void TYPE##Manager_AddComponent(TYPE component);       \
                                                           \
    void TYPE##Manager_RemoveComponent(EntityId entityId); \
                                                           \
    TYPE *TYPE##Manager_GetComponent(EntityId entityId);

#define COMPONENT_MANAGER_SOURCE(TYPE, COMPONENT_ID)                                         \
    static TYPE *componentArray##TYPE;                                                       \
    static Sint16 lastPosition##TYPE = -1;                                                   \
    static Sint16 maxPosition##TYPE = 0;                                                     \
                                                                                             \
    void TYPE##Manager_Initialize(Sint16 maxComponents)                                      \
    {                                                                                        \
        maxPosition##TYPE = maxComponents - 1;                                               \
        componentArray##TYPE = (TYPE *)Memmalloc(sizeof(TYPE) * maxComponents);              \
    }                                                                                        \
                                                                                             \
    void TYPE##Manager_AddComponent(TYPE component)                                          \
    {                                                                                        \
        if (lastPosition##TYPE++ > maxPosition##TYPE)                                        \
        {                                                                                    \
            lastPosition##TYPE = maxPosition##TYPE;                                          \
            return;                                                                          \
        }                                                                                    \
        componentArray##TYPE[lastPosition##TYPE] = component;                                \
                                                                                             \
        EntitySystem_SetComponentId(component.entityId, COMPONENT_ID, lastPosition##TYPE);   \
    }                                                                                        \
                                                                                             \
    void TYPE##Manager_RemoveComponent(EntityId entityId)                                    \
    {                                                                                        \
        ComponentId componentId = EntitySystem_GetComponentId(entityId, COMPONENT_ID);       \
        if (!componentId)                                                                    \
            return;                                                                          \
                                                                                             \
        if (!componentId == lastPosition##TYPE)                                              \
        {                                                                                    \
            componentArray##TYPE[componentId] = componentArray##TYPE[lastPosition##TYPE];    \
            EntitySystem_SetComponentId(entityId, COMPONENT_ID, componentId);                \
        }                                                                                    \
                                                                                             \
        EntitySystem_SetComponentId(entityId, COMPONENT_ID, UNUSED_COMPONENT);               \
                                                                                             \
        lastPosition##TYPE--;                                                                \
    }                                                                                        \
                                                                                             \
    TYPE *TYPE##Manager_GetComponent(EntityId entityId)                                      \
    {                                                                                        \
        return &(componentArray##TYPE[EntitySystem_GetComponentId(entityId, COMPONENT_ID)]); \
    }

#endif // !COMPONENT_MANAGER_TEMPLATE_H
