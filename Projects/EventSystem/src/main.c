#include <sgl.h>
#include "Utils/mem_mgr.h"

#include "EventSystem/EventManager.h"

void init()
{
    slInitSystem(TV_320x240, NULL, 1);
    slPerspective(DEGtoANG(60.0));

    // use 512K of work RAM for general heap
    // handle set size = 16, trash RAM = 2K
    MemInit((pointer)0x06040000, (pointer)0x060BFFFF, 16, 16, 0x800);
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

    EventManager_Init();
    EventManager_AddListener(TestEvent, &exampleEventListener);

    while (1)
    {
        EventManager_QueueEvent(TestEvent, (void *)3);
        EventManager_QueueEvent(TestEvent, (void *)4);

        EventManager_AbortEvent(TestEvent, 1);

        EventManager_TriggerEvent(TestEvent, (void *)1);
        EventManager_TriggerEvent(TestEvent, (void *)2);

        EventManager_Update();
        slSynch();
    }
    return 1;
}
