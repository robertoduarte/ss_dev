#ifndef COMPONENT_MANAGER_H
#define COMPONENT_MANAGER_H
#include <SL_DEF.H>
//#define CM_UNSAFE

typedef struct ComponentManager ComponentManager;

typedef void (*Callback)(void *component, short entityId);

#define COMPONENT_LAMBDA(type, body) \
    ({ void COMPONENT_LAMBDA(void *c, short entityId) {type* component = c; body} COMPONENT_LAMBDA; })

ComponentManager *New_ComponentManager(short componentSize, short entityCapacity, short componentCapacity);

void ComponentManager_Delete(ComponentManager *manager);

Bool ComponentManager_CreateComponent(ComponentManager *manager, short entityId);

int ComponentManager_CheckEntity(ComponentManager *manager, short entityId);

void *ComponentManager_GetComponent(ComponentManager *manager, short entityId);

void ComponentManager_RemoveComponent(ComponentManager *manager, short entityId);

void ComponentManager_UpdateEntityId(ComponentManager *manager, short oldId, short newId);

short ComponentManager_First(ComponentManager *manager);

short ComponentManager_Next(ComponentManager *manager, short entityId);

Bool ComponentManager_Done(ComponentManager *manager, short entityId);

void ComponentManager_Foreach(ComponentManager *manager, Callback callback);

#endif // !COMPONENT_MANAGER_H
