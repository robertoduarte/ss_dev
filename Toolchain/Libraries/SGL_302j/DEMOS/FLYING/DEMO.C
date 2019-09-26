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

long demoClock = 0;
int demoRunning = FALSE;
extern int rendPatchesRendered;
extern int rendPatchesExtant;

int DemoInit( void )
{
	long heap;

	if (RendInit() == FALSE)
		return( FALSE );

	ActCreateActorTree();

	// use 512K of work RAM for general heap
	// handle set size = 16, trash RAM = 2K

	MemInit( (pointer)0x06040000, (pointer)0x060BFFFF, 16, 16, 0x800 );

	demoRunning = TRUE;	

	AIDemoInit();

	return( TRUE );
}

void DemoCycle( int clock )
{
	assert( demoRunning == TRUE );
	demoClock = clock;
	ActUpdate( clock );	
	CollideUpdate();
	RendUpdate( clock );
}

void DemoShutdown( void )
{
	RendShutdown();
	ActKillActorTree();
}
