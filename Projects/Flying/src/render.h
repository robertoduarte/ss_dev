//------------------------------------------------------------------------
//
//	RENDER.H
//	Rendering engine header
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

#ifndef _RENDER_H_
#define _RENDER_H_

//------------------- PRE-INCLUDE ENVIRONMENT SETTINGS -------------------

//------------------------------- INCLUDES -------------------------------

#include "enigma.h"

//-------------------------- MACROS & TYPEDEFS ---------------------------

#define RENDER_NEAR_Z_CLIP 10.0

#define X_DIST_TO_LENS  2.0
#define Y_DIST_TO_LENS  2.0

//--------------------------- GLOBAL VARIABLES ---------------------------

extern EOBJECT* rendCamera;
extern EOBJECT* rendCameraTarget;
//--------------------------- GLOBAL FUNCTIONS ---------------------------

int RendInit(void);
void RendUpdate(int clock);
void RendShutdown(void);
void DebugEntry(int index, int data);
void DebugCheckpoint(int index);
long DebugTimeMark(void);
void DebugClock(int clock);

#endif


