#include <sgl.h>
#include "Utils/mem_mgr.h"
#include "Utils/rbtree.h"

#include "EventSystem/EventManager.h"
#include "ECS/ComponentManagers.h"

void init()
{
    slInitSystem(TV_320x240, NULL, 1);
    slPerspective(DEGtoANG(60.0));
    // handle set size = 16, trash RAM = 2K
    MemInit((pointer)0x06040000, (pointer)0x060BFFFF, 16, 16, 0x800);
    EntitySystem_Initialize();
    HealthManager_Initialize(10);
    MotionManager_Initialize(10);
    TransformManager_Initialize(10);
}

void exampleEventListener(Event *event)
{
    switch (event->type)
    {
    case TestEvent:
        slPrintFX(toFIXED((int)event->arg), slLocate(0, (int)event->arg));
    }
}
int main(void)
{
    init();

    //EventManager_Init();
    //EventManager_AddListener(TestEvent, &exampleEventListener);

    EntitySystem_AssignId();
    EntitySystem_AssignId();
    EntityId test = EntitySystem_AssignId();
    EntitySystem_AssignId();
    EntitySystem_FreeId(test);

    EntityId entityId = EntitySystem_AssignId();

    slPrintFX(toFIXED(entityId), slLocate(0, 0));

    Health hC = {entityId, 10, 20};

    HealthManager_AddComponent(hC);
    HealthManager_RemoveComponent(entityId);

    slPrintFX(toFIXED(HealthManager_GetComponent(entityId)->currentHealth), slLocate(0, 1));

    return 1;
}
