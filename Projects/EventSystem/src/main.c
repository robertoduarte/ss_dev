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
    case EVENT_TEST:
        slPrintFX(toFIXED((int)event->arg), slLocate(0, (int)event->arg));
    }
}

int main(void)
{
    init();

    EventManager_Init();
    EventManager_AddListener(EVENT_TEST, &exampleEventListener);

    while (1)
    {
        EventManager_QueueEvent(EVENT_TEST, (void *)3);
        EventManager_QueueEvent(EVENT_TEST, (void *)4);

        EventManager_AbortEvent(EVENT_TEST, 0);
        
        EventManager_TriggerEvent(EVENT_TEST, (void *)1);
        EventManager_TriggerEvent(EVENT_TEST, (void *)2);

        EventManager_Update();
        slSynch();
    }
    return 1;
}
