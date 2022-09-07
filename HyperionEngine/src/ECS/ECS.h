#pragma once

#include "Components.h"

#define ENTITY_CAPACITY 512

typedef short EntityId;

typedef unsigned int ArchetypeId;

typedef struct ComponentAccessor ComponentAccessor;

typedef void (*ECS_Callback)(ComponentAccessor *accessor, EntityId selfId);

#define LAMBDA(body) ({ void callback(ComponentAccessor *accessor, EntityId id){body} callback; })

void ECS_Init();

EntityId ECS_CreateEntity(ArchetypeId components);

void ECS_AddComponents(EntityId entityId, ArchetypeId components);

void ECS_RemoveComponents(EntityId entityId, ArchetypeId components);

void ECS_ForEach(ArchetypeId components, ECS_Callback callback);

#define AS_COMPONENT_GETTER(type) type *ECS_Get_##type(EntityId entityId);
TYPES(AS_COMPONENT_GETTER)

#define AS_COMPONENT_ACCESSOR(type) type *ECS_Access_##type(ComponentAccessor *accessor);
TYPES(AS_COMPONENT_ACCESSOR)
