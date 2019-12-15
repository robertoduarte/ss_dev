#ifndef COMPONENT_MANAGER_H
#define COMPONENT_MANAGER_H

typedef struct ComponentManager ComponentManager;

ComponentManager *ComponentManager_New(short componentSize, short entityCapacity, short componentCapacity);

void ComponentManager_Delete(ComponentManager *manager);

short ComponentManager_AssignComponent(ComponentManager *manager, short entityId);

short ComponentManager_EntityFromComponent(ComponentManager *manager, short componentId);

short ComponentManager_ComponentFromEntity(ComponentManager *manager, short entityId);

void *ComponentManager_ComponentAt(ComponentManager *manager, short componentId);

void ComponentManager_RemoveComponent(ComponentManager *manager, short componentId);

void ComponentManager_ClearComponent(ComponentManager *manager, short componentId);

void ComponentManager_UpdateEntity(ComponentManager *manager, short oldId, short newId);

#endif // !COMPONENT_MANAGER_H
