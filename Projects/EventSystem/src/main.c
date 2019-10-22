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

int doSum(int a, int b)
{
    return a + b;
}

void exampleEventListener(Event *event)
{

    int result;
    switch (event->type)
    {
    case E_NEWGAMEEASY:
        result = doSum(*(int *)event->arg, 2);
    }
}

int main(void)
{
    init();

    int testInt = 1;

    EventManager_Init();
    EventManager_AddListener(E_NEWGAMEEASY, &exampleEventListener);
    EventManager_TriggerEvent(E_NEWGAMEEASY, &testInt);

    while (1)
    {
        slPrint("Just WORKS!", slLocate(0, 0));
        slSynch();
    }
    return 1;
}
