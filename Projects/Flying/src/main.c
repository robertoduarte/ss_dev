//------------------------------------------------------------------------
//
//	MAIN.C
//  Entry point after cinit startup code.
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

#include "sgl.h"
#include "demo.h"

void ss_main(void)
{
    Demo demo;
    
    DemoInit(&demo);
    
    while (1)
        DemoCycle(&demo);
}
