//------------------------------------------------------------------------
//
//	MASTER.H
//  Master configuration control
//
//	CONFIDENTIAL
//	Copyright (c) 1995, Sega Technical Institute
//
//	AUTHOR
//  Russell Bornsch++, 10/95
//
//	TARGET
//	GCC for SH2
//
//	REVISION
//	  8/7/96 - RAB - Cleanup for demo release
//
//------------------------------------------------------------------------

#ifndef _MASTER_H
#define _MASTER_H

// target platform is PC or Saturn?

#define _TARGET_PC	0
#define _TARGET_SAT 1

// target platform is NTSC or PAL video? (ignored on PC)

#define _TARGET_NTSC	1
#define _TARGET_PAL		0

// idle resources are cached, or freed?

#define _IDLE_CACHE	1
#define _IDLE_FREE	0

// on PC, resources are loaded from disk on demand, or all at once?

#define _RES_PRELOAD	0
#define _RES_DEMAND		1

// resource manager provides handles or pointers?

#define _RES_HANDLE		0
#define _RES_POINTER	1

// enable compression code for testing compress/decompress accuracy

#define _COMPRESS		1

// #undef NDEBUG to enable debugging assertions
// #define NDEBUG to disable assertions

#undef NDEBUG
//#define NDEBUG			1

#pragma warning ( disable : 4245 )

#endif
