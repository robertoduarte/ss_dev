#include "EntityManager.h"
#include "Component.h"
#include "ComponentMap.h"
#include "ComponentContainer.h"
#include "../Utils/SatMalloc.h"

typedef struct EntityMap
{
    short archetypeIndex;
    short containerIndex;
} EntityMap;

EntityMap *g_entityMaps;

static void EntityMap_Init()
{
    if (!g_entityMaps)
    {
        g_entityMaps = malloc(sizeof(EntityMap) * ENTITY_CAPACITY);
        for (size_t i = 0; i < ENTITY_CAPACITY; i++)
        {
            g_entityMaps[i].archetypeIndex = -1;
            g_entityMaps[i].containerIndex = -1;
        }
    }
}

static void EntityMap_Set(EntityId entityId, short archetypeIndex, short containerIndex)
{
    EntityMap_Init();
    g_entityMaps[entityId].archetypeIndex = archetypeIndex;
    g_entityMaps[entityId].containerIndex = containerIndex;
}

static EntityMap *EntityMap_Get(EntityId entityId)
{
    EntityMap_Init();
    if (g_entityMaps[entityId].archetypeIndex > 0)
        return &g_entityMaps[entityId];
    else
        return NULL;
}

static size_t ArchetypeSize(ArchetypeId archetypeId)
{
    size_t totalSize = 0;
    for (Component i = 0; i < COUNT(COMPONENTS); i++)
        if (archetypeId & ComponentGetId(i))
            totalSize += ComponentGetSize(i);
    return totalSize;
}

typedef struct Archetype
{
    ArchetypeId id;
    ComponentContainerVector *componentContainers;
} Archetype;

VECT_GENERATE_TYPE(Archetype);

static ArchetypeVector *g_archetypes;

int Archetype_GetIndex(ArchetypeId archetypeId)
{
    if (!g_archetypes)
        g_archetypes = ArchetypeVector_Init(1);

    int archetypeIndex = -1;

    for (size_t i = 0; i < g_archetypes->size; i++)
    {
        if (ArchetypeVector_Pointer(g_archetypes, i)->id == archetypeId)
        {
            archetypeIndex = i;
            break;
        }
    }

    if (archetypeIndex < 0)
    {
        Debug_PrintMsg("New Archetype!");
        archetypeIndex = g_archetypes->size;
        Archetype *archetype = ArchetypeVector_PushAndGetPointer(g_archetypes);
        archetype->id = archetypeId;
        archetype->componentContainers = ComponentContainerVector_Init(ArchetypeSize(archetypeId));
    }

    return archetypeIndex;
}

EntityId EntityManager_CreateEntity(ArchetypeId archetypeId)
{
    EntityId entityId = EntityId_CreateId();
    if (entityId >= 0)
    {
        int archetypeIndex = Archetype_GetIndex(archetypeId);
        Archetype *archetype = &g_archetypes->data[archetypeIndex];
        int containerIndex = archetype->componentContainers->size;
        ComponentContainerVector_Push(archetype->componentContainers, entityId);
        EntityMap_Set(entityId, archetypeIndex, containerIndex);
    }
    return entityId;
}
