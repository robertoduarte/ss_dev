#ifndef COMPONENT_MANAGER_H
#define COMPONENT_MANAGER_H

typedef struct ComponentManager ComponentManager;

ComponentManager *New_ComponentManager(short componentSize, short entityCapacity, short componentCapacity);

void ComponentManager_Delete(ComponentManager *manager);

short ComponentManager_AssignComponent(ComponentManager *manager, short entityId);

short ComponentManager_EntityFromComponent(ComponentManager *manager, short componentId);

short ComponentManager_ComponentFromEntity(ComponentManager *manager, short entityId);

void *ComponentManager_ComponentAt(ComponentManager *manager, short componentId);

void ComponentManager_RemoveComponent(ComponentManager *manager, short componentId);

void ComponentManager_RemoveComponentByEntity(ComponentManager *manager, short entityId);

void ComponentManager_UpdateEntity(ComponentManager *manager, short oldId, short newId);

#endif // !COMPONENT_MANAGER_H
