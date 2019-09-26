//------------------------------------------------------------------------
//
//	DEMO.C
//	Enigma actor engine demo
//
//	CONFIDENTIAL
//	Copyright (c) 1996, Sega Technical Institute
//
//	AUTHOR
//  Russell Bornsch++, 4/96
//
//	TARGET
//	GCC for SH2
//
//	REVISION
//	  8/7/96 - RAB - Cleanup for demo release
//
//------------------------------------------------------------------------

#include <stdio.h>
#include "enigma.h"
#include "render.h"
#include "actor.h"
#include "demo.h"
#include "control.h"
#include "mem_mgr.h"
#include "assert.h"
#include "ai.h"
#include "collide.h"

int DemoInit(Demo* demo)
{
    if (!RendInit()) return FALSE;

    ActCreateActorTree();

    // use 512K of work RAM for general heap
    // handle set size = 16, trash RAM = 2K

    MemInit((pointer) 0x06040000, (pointer) 0x060BFFFF, 16, 16, 0x800);
    demo->clock = 0;
    demo->running = TRUE;
    AIDemoInit();

    return TRUE;
}

void DemoCycle(Demo* demo)
{
    assert(demo->running == TRUE);
    demo->clock++;
    ActUpdate(demo->clock);
    CollideUpdate();
    RendUpdate(demo->clock);
}

void DemoShutdown()
{
    RendShutdown();
    ActKillActorTree();
}
