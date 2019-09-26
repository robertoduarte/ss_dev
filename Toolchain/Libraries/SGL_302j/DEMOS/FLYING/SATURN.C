//------------------------------------------------------------------------
//
//	SATURN.H
//	Saturn-specific hardware functions
//
//	CONFIDENTIAL
//	Copyright (c) 1995-1996, Sega Technical Institute
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

#include "saturn.h"
#include "assert.h"

static long satTime = 0;

void SatInitTime( void )
{
	unsigned char tcsr;

	*SH2_TCR = SH2_TCR_CKS1;	// í/128 clock - 200KHz?

	// clear overflow
	*SH2_TCSR = 0;
	// clear counter
	*SH2_FRC = 0;
	// clear time
	satTime = 0;
}

// this is usable as long as you never go more than about
// 2/3 of a second between SatGetTime calls

long SatGetTime( void )
{
	unsigned long e;

	e = *SH2_FRC;
	if (*SH2_TCSR & SH2_TCSR_OVF)
		e |= 0x10000;
	// clear overflow
	*SH2_TCSR = 0;
	// clear counter
	*SH2_FRC = 0;

	satTime += e;
	return( satTime );
}

/*
	assert handler

*/				  

void _sassert( char* message1, char* message2 )
{

	slPrint( "Assertion failed:", slLocate( 2, 23 ) );
	slPrint( message1, slLocate( 2, 24 ) );
	slPrint( message2, slLocate( 2, 25 ) );
	slSynch();

	asm("trapa	#12");
}


		 

