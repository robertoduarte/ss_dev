#include "HealthComponentManager.h"
#include "../Utils/mem_mgr.h"
#include "../Utils/map.h"

static Map entityComponentMap;
static HealthComponent *healthComponentArray;
static Sint32 lastPosition = 0;
static Sint32 maxPositions = 0;

#define ActualIndex(index) (index - 1)

void HealthManager_Initialize(Uint16 max)
{
    maxPositions = max;
    healthComponentArray = (HealthComponent *)Memmalloc(sizeof(HealthComponent) * maxPositions);
}

void HealthManager_AddComponent(Uint16 entityID, HealthComponent healthComponent)
{
    if (lastPosition++ > maxPositions)
        return; // TODO! handle error!
    healthComponentArray[ActualIndex(lastPosition)] = healthComponent;
    Map_Set(entityComponentMap, entityID, lastPosition);
}
void HealthManager_TestComponent(Uint16 entityID)
{
    Sint32 componentIndex = Map_Get(entityComponentMap, entityID);
    slPrintFX(toFIXED(healthComponentArray[ActualIndex(componentIndex)].currentHealth), slLocate(0, 0));
}

void HealthManager_RemoveComponent(Uint16 entityID)
{
    Sint32 componentIndex;
    if (componentIndex = Map_Get(entityComponentMap, entityID))
    {
        healthComponentArray[ActualIndex(componentIndex)] = healthComponentArray[ActualIndex(lastPosition)];
        lastPosition--;
        Map_Remove(entityComponentMap, entityID);
    }
}
