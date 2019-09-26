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

typedef struct
{
    long clock;
    int running;
} Demo;


int DemoInit(Demo* demo);
void DemoCycle(Demo* demo);
void DemoShutdown();
