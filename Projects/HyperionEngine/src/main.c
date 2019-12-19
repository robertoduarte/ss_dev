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

    ComponentManager *healthManager = New_ComponentManager(sizeof(Health), 10, 10);

    ComponentManager_AssignComponent(healthManager, 1);
    short componentId = ComponentManager_AssignComponent(healthManager, 2);

    slPrintFX(toFIXED(ComponentManager_EntityFromComponent(healthManager, componentId)), slLocate(0, 0));

    Health *healthPtr = ComponentManager_ComponentAt(healthManager, componentId);

    healthPtr->current = 9;
    healthPtr->max = 10;

    ComponentManager_UpdateEntity(healthManager, 2, 3);

    healthPtr = ComponentManager_ComponentAt(healthManager, ComponentManager_ComponentFromEntity(healthManager, 3));

    slPrintFX(toFIXED(healthPtr->current), slLocate(0, 1));
    slPrintFX(toFIXED(sizeof(Health)), slLocate(0, 2));
    
    return 1;
}
