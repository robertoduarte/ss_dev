//------------------------------------------------------------------------
//
//	ACTOR.H
//	Enigma actor engine stuff
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

#ifndef _ACTOR_H_
#define _ACTOR_H_

//------------------- PRE-INCLUDE ENVIRONMENT SETTINGS -------------------

//------------------------------- INCLUDES -------------------------------

#include "enigma.h"

//-------------------------- MACROS & TYPEDEFS ---------------------------

#define ACTOR_MEM_ALLOC(size)  Memmalloc(size)
#define ACTOR_MEM_FREE(ptr)   Memfree(ptr)

// these are muchly approximate, assuming you can maintain 60Hz cycling

#define DELAY_60HZ  1
#define DELAY_30HZ  2
#define DELAY_20HZ  3
#define DELAY_15HZ  4
#define DELAY_12HZ  5
#define DELAY_10HZ  6
#define DELAY_9HZ  7
#define DELAY_8HZ  8
#define DELAY_7HZ  9
#define DELAY_6HZ  10
#define DELAY_5HZ  12
#define DELAY_4HZ  15
#define DELAY_3HZ  20
#define DELAY_2HZ  30
#define DELAY_1HZ  60

//--------------------------- GLOBAL VARIABLES ---------------------------

extern char* act_xyz_order;
extern char* act_xzy_order;
extern char* act_yxz_order;
extern char* act_yzx_order;
extern char* act_zxy_order;
extern char* act_zyx_order;

//--------------------------- GLOBAL FUNCTIONS ---------------------------

extern void ActSendMessage(ACTOR* receive, MESSAGE_FLAGS mess, long data);
extern void ActCreateActorTree(void);
extern void ActKillActorTree(void);
extern ACTOR* ActCreateActor(
                             char* actor_name,
                             ACTOR_FLAGS flags,
                             short priority,
                             MESSAGE_HAND message_handler,
                             ACTOR* parent,
                             int scratch);
extern PROCESS* ActAddProcess(
                              ACTOR* actor,
                              PROCESS_FUNC function,
                              int delay,
                              int autodelay,
                              long priority);
extern EOBJECT* ActAddObject(
                             ACTOR* actor,
                             OBJECT_FLAGS flags,
                             int num_verts,
                             int num_patches);
extern int ActUpdate(long clock);
extern void ProcSetDelay(PROCESS* proc, int delay, int autodelay);
extern void ProcSetFunction(PROCESS* proc, PROCESS_FUNC function);
extern void ProcSetPriority(ACTOR* actor, PROCESS* proc, int priority);
extern EOBJECT* ActGetFirstObject(ACTOR* act);
extern EOBJECT* ActGetNextObject(ACTOR* act, EOBJECT* obj);

#endif

