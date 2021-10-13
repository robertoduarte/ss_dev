#include "ECS.h"

#include "../Utils/Debug.h"
#include "../Utils/RBTree.h"
#include "../Utils/Vector.h"
#include "../Utils/SatMalloc.h"
#include "../Utils/IdGenerator.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

/*----------- Compilation error check -----------*/

/* Error if number of components is greater what fits in ArchetypeId. */
MACRO_ASSERT(COUNT(TYPES) > CHAR_BIT * sizeof(ArchetypeId))

/*-------------- Type Definitions --------------*/

VECT_GENERATE_TYPE(EntityId)

typedef struct ComponentMap
{
    size_t pos[COUNT(TYPES)];
} ComponentMap;

struct ComponentAccessor
{
    void *ptr;
    ComponentMap *map;
};

typedef struct
{
    ArchetypeId archetypeId;
    ComponentMap map;
    EntityIdVector *entityIds;
    Vector *types;
} ArchetypeManager;

typedef struct
{
    ArchetypeManager *manager;
    int row;
} Record;

/*------------------ Globals -------------------*/

static IdGenerator *g_idGenerator = NULL;
static RBNode *g_archetypes = NULL;
static Record *g_entityRecords = NULL;
static bool g_init = false;

/*------------------- Macros -------------------*/

#define INIT_CHECK \
    if (!g_init)   \
    Debug_ExitMsg("ECS Not Initialized!")

#define AS_ARCHETYPE_SIZE_SUM(type, archetype) (archetype & AS_ID(type) ? sizeof(type) : 0) +
#define ARCHETYPE_SIZE(archetype) (TYPES(AS_ARCHETYPE_SIZE_SUM, archetype) 0)

#define AS_POS_SUM(type, archetype, component) \
    (AS_COMPONENT(type) < component && (archetype & AS_ID(type)) ? sizeof(type) : 0) +
#define COMPONENT_POSITION(archetype, component) (TYPES(AS_POS_SUM, archetype, component) 0)

/*----------------------------------------------*/

void ECS_Init()
{
    if (!g_idGenerator)
        g_idGenerator = IdGenerator_Init(ENTITY_CAPACITY);

    if (!g_entityRecords)
        g_entityRecords = malloc(sizeof(Record) * ENTITY_CAPACITY);

    g_init = true;
}

static void ArchetypeInit(ArchetypeManager *archetype, ArchetypeId type)
{
    archetype->archetypeId = type;
    for (int i = 0; i < COUNT(TYPES); i++)
    {
        archetype->map.pos[i] = COMPONENT_POSITION(type, i);
    }
    archetype->entityIds = EntityIdVector_Init(1);
    archetype->types = Vector_Init(ARCHETYPE_SIZE(type), 1);
}

static ArchetypeManager *GetArchetype(ArchetypeId type)
{
    ArchetypeManager *archetype = (ArchetypeManager *)RBTree_Search(g_archetypes, type);
    if (!archetype)
    {
        archetype = (ArchetypeManager *)RBTree_AllocAndInsert(&g_archetypes, type, sizeof(ArchetypeManager));
        ArchetypeInit(archetype, type);
    }
    return archetype;
}

static int Archetype_AddEntity(ArchetypeManager *archetype, EntityId entityId)
{
    Vector_Push(archetype->types);
    EntityIdVector_Push(archetype->entityIds, entityId);
    return archetype->entityIds->size - 1;
}

static void Archetype_RemoveEntity(ArchetypeManager *archetype, EntityId entityId)
{
    int row = g_entityRecords[entityId].row;
    int lastRow = archetype->types->size - 1;

    if (row != lastRow)
    {
        void *rowPtr = Vector_GetPtrAt(archetype->types, row);
        void *lastRowPtr = Vector_GetPtrAt(archetype->types, lastRow);

        memcpy(rowPtr, lastRowPtr, archetype->types->dataSize);

        EntityId lastRowEntityId = EntityIdVector_At(archetype->entityIds, lastRow);

        g_entityRecords[lastRowEntityId].row = row;
    }

    archetype->entityIds->size--;
    archetype->types->size--;
}

static bool Archetype_HasComponent(ArchetypeId type, Component component)
{
    return type & (1 << component);
}

void *AccessComponent(ComponentAccessor *accessor, Component component)
{
    INIT_CHECK;
    return accessor->ptr + accessor->map->pos[component];
}

#define AS_ACCESSOR_DEF(type)                                         \
    type *ECS_Access_##type(ComponentAccessor *accessor)              \
    {                                                                 \
        return (type *)AccessComponent(accessor, AS_COMPONENT(type)); \
    }

TYPES(AS_ACCESSOR_DEF)

static void *ComponentAt(ArchetypeManager *manager, int row, Component component)
{
    ComponentAccessor accessor = {Vector_GetPtrAt(manager->types, row),
                                  &manager->map};
    return AccessComponent(&accessor, component);
}

static void ComponentCopy(ArchetypeManager *srcArchetype, int srcRow, ArchetypeManager *destArchetype, int destRow)
{
#define AS_COPY_COMPONENTS(type)                                                 \
    if (Archetype_HasComponent(srcArchetype->archetypeId, AS_COMPONENT(type)) && \
        Archetype_HasComponent(destArchetype->archetypeId, AS_COMPONENT(type)))  \
        *(type *)ComponentAt(destArchetype, destRow, AS_COMPONENT(type)) =       \
            *(type *)ComponentAt(srcArchetype, srcRow, AS_COMPONENT(type));
    TYPES(AS_COPY_COMPONENTS)
}

static void ChangeComponents(EntityId entityId, ArchetypeId types, bool remove)
{
    INIT_CHECK;
    if (!types)
        return;
    Record *record = &g_entityRecords[entityId];
    if (record->manager)
    {
        bool hascomponents = ((record->manager->archetypeId & types) == types);
        if ((remove && !hascomponents) || (!remove && hascomponents))
            return; //nothing to do

        // components change
        ArchetypeId newArchetype = remove ? record->manager->archetypeId & ~types : record->manager->archetypeId | types;
        if (newArchetype)
        {
            ArchetypeManager *destArchetypeMgr = GetArchetype(newArchetype);
            int newRow = Archetype_AddEntity(destArchetypeMgr, entityId);
            ComponentCopy(record->manager, record->row, destArchetypeMgr, newRow);
            Archetype_RemoveEntity(record->manager, entityId);
            record->manager = destArchetypeMgr;
            record->row = newRow;
        }
        else
        {
            Archetype_RemoveEntity(record->manager, entityId);
            record->manager = NULL;
            record->row = -1;
        }
    }
    else
    {
        // New Entity
        record->manager = GetArchetype(types);
        record->row = Archetype_AddEntity(record->manager, entityId);
    }
}

void ECS_AddComponents(EntityId entityId, ArchetypeId types)
{
    ChangeComponents(entityId, types, false);
}

void ECS_RemoveComponents(EntityId entityId, ArchetypeId types)
{
    ChangeComponents(entityId, types, true);
}

void *ECS_GetComponent(EntityId entityId, Component component)
{
    INIT_CHECK;
    Record *record = &g_entityRecords[entityId];
    if (record->manager &&
        Archetype_HasComponent(record->manager->archetypeId, component))
        return ComponentAt(record->manager, record->row, component);
    else
        return NULL;
}

#define AS_GETTER_DEF(type)                                            \
    type *ECS_Get_##type(EntityId entityId)                            \
    {                                                                  \
        return (type *)ECS_GetComponent(entityId, AS_COMPONENT(type)); \
    }

TYPES(AS_GETTER_DEF)

EntityId ECS_CreateEntity(ArchetypeId types)
{
    INIT_CHECK;
    if (!types)
        Debug_ExitMsg("Trying to create entity without types!");

    EntityId id = IdGenerator_GenerateId(g_idGenerator);
    ECS_AddComponents(id, types);
    return id;
}

static void Process(RBNode *node, ArchetypeId components, ECS_Callback callback)
{
    if (node == NULL)
        return;

    Process(node->left, components, callback);
    Process(node->right, components, callback);
    ArchetypeManager *manager = (ArchetypeManager *)node->data;
    if ((manager->archetypeId & components) == components)
    {
        ComponentAccessor accessor;
        for (int i = 0; i < manager->entityIds->size; i++)
        {
            accessor.ptr = Vector_GetPtrAt(manager->types, i);
            accessor.map = &manager->map;
            callback(&accessor, EntityIdVector_At(manager->entityIds, i));
        }
    }
}

void ECS_ForEach(ArchetypeId components, ECS_Callback callback)
{
    Process(g_archetypes, components, callback);
}
