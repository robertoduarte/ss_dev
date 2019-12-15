#include <sgl.h>
#include "EventSystem/EventManager.h"
#include "ECS\ComponentManager.h"
#include "ECS\Components.h"

void init()
{
    slInitSystem(TV_320x240, NULL, 1);
    slPerspective(DEGtoANG(60.0));
    //EntitySystem_Initialize();
}

int main(void)
{
    init();

    ComponentManager *healthManager = ComponentManager_New(sizeof(Health), 10, 10);

    ComponentManager_AssignComponent(healthManager, 1);
    short componentId = ComponentManager_AssignComponent(healthManager, 2);

    slPrintFX(toFIXED(ComponentManager_EntityFromComponent(healthManager, componentId)), slLocate(0, 0));

    Health *healthPtr = ComponentManager_ComponentAt(healthManager, componentId);

    healthPtr->current = 9;
    healthPtr->max = 10;

    ComponentManager_UpdateEntity(healthManager, 2, 3);

    healthPtr = ComponentManager_ComponentAt(healthManager, ComponentManager_ComponentFromEntity(healthManager, 3));

    slPrintFX(toFIXED(healthPtr->current), slLocate(0, 1));

    return 1;
}
