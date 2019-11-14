#include "HealthComponentManager.h"
#include "../Utils/mem_mgr.h"
#include "../Utils/rbtree.h"

static RedBlackNode *entityComponentMap;
static HealthComponent *healthComponentArray;
static Sint32 lastComponentIndex = -1;
static Sint32 _maxComponents = 0;

void HealthManager_Initialize(Sint32 maxComponents)
{
    _maxComponents = maxComponents;
    healthComponentArray = (HealthComponent *)Memmalloc(sizeof(HealthComponent) * maxComponents);
}

void HealthManager_AddComponent(Uint16 entityID, HealthComponent healthComponent)
{
    lastComponentIndex++;
    healthComponentArray[lastComponentIndex] = healthComponent;
    Insert(&entityComponentMap, entityID, lastComponentIndex);
}
void HealthManager_UpdateComponent(Uint16 entityID)
{
    Sint32 componentIndex = GetData(entityComponentMap, entityID);
    slPrintFX(toFIXED(healthComponentArray[componentIndex].currentHealth), slLocate(0, 0));
}
