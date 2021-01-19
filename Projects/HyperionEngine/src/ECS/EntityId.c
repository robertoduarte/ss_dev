#include "EntityId.h"
#include "..\EventSystem\EventManager.h"

#ifndef ENTITY_CAPACITY
#define ENTITY_CAPACITY 256
#endif // !ENTITY_CAPACITY

#define WORD_SIZE 32
#define FULL_WORD 0xFFFFFFFF
#define TOTAL_WORDS ((ENTITY_CAPACITY / WORD_SIZE) + 1)

#define SET_BIT(A, k) (A[(k / WORD_SIZE)] |= (1 << (k % WORD_SIZE)))
#define CLEAR_BIT(A, k) (A[(k / WORD_SIZE)] &= ~(1 << (k % WORD_SIZE)))
#define TEST_BIT(A, k) (A[(k / WORD_SIZE)] & (1 << (k % WORD_SIZE)))

static unsigned int g_entityBits[TOTAL_WORDS];
static bool g_init = false;

static void FreeId(EntityId entityId)
{
    if (entityId >= 0 && entityId < ENTITY_CAPACITY)
        CLEAR_BIT(g_entityBits, entityId);
}

static void Listener(EventType type, unsigned int arg)
{
    switch (type)
    {
    case (Entity_Destroy):
        FreeId((EntityId)arg);
        break;
    default:
        break;
    }
}

static void Init()
{
    EventManager_AddListener(Entity_Destroy, Listener);
    g_init = true;
}

EntityId EntityId_CreateId()
{
    if (!g_init)
        Init();

    int skippedWords = 0;
    while (g_entityBits[skippedWords] == FULL_WORD && skippedWords < TOTAL_WORDS)
    {
        skippedWords++;
    }
    int nextEntityBit = skippedWords * WORD_SIZE;
    while (nextEntityBit < ENTITY_CAPACITY && TEST_BIT(g_entityBits, nextEntityBit))
    {
        nextEntityBit++;
    }
    if (nextEntityBit >= ENTITY_CAPACITY)
    {
        return -1;
    }
    else
    {
        SET_BIT(g_entityBits, nextEntityBit);
        return nextEntityBit;
    }
}
