#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include "ArchetypeId.h"
#include "EntityId.h"
#include "Component.h"
#include <stdbool.h>
#include <stdlib.h>

EntityId EntityManager_CreateEntity(ArchetypeId archetypeId);

#endif // !ENTITY_MANAGER_H
