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

extern long demoClock;
extern int demoRunning;

int DemoInit( void );
void DemoCycle( int clock );
void DemoShutdown( void );
