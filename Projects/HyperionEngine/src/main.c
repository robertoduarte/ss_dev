#include <SGL.H>
#include "EventSystem/EventManager.h"
#include "ECS\ComponentManager.h"
#include "ECS\Components.h"
#include <..\..\sh-coff\include\malloc.h>

void init()
{
    slInitSystem(TV_320x240, NULL, 2);
    slPerspective(DEGtoANG(60.0));
}

int main(void)
{
    init();

    ComponentManager *healthManager = New_ComponentManager(sizeof(Health), 30, 10);

    ComponentManager_CreateComponent(healthManager, 1);
    ComponentManager_CreateComponent(healthManager, 4);
    ComponentManager_CreateComponent(healthManager, 20);
    ComponentManager_CreateComponent(healthManager, 9);
    ComponentManager_CreateComponent(healthManager, 7);
    ComponentManager_CreateComponent(healthManager, 12);

    int line = 0;
    for (short entityId = ComponentManager_First(healthManager); !ComponentManager_Done(healthManager, entityId); entityId = ComponentManager_Next(healthManager, entityId))
    {
        slPrintFX(toFIXED(entityId), slLocate(0, ++line));
        ((Health*)ComponentManager_GetComponent(healthManager, entityId))->current=line+1;
    }
    for (short entityId = ComponentManager_First(healthManager); !ComponentManager_Done(healthManager, entityId); entityId = ComponentManager_Next(healthManager, entityId))
    {
        slPrintFX(toFIXED(((Health *)ComponentManager_GetComponent(healthManager, entityId))->current), slLocate(0, ++line));
    }

    return 1;
}
