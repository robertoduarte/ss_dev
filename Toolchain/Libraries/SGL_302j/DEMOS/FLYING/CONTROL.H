//------------------------------------------------------------------------
//
//	CONTROL.H
//	Interface to controller pads
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

#ifndef _CONTROL_H_
#define _CONTROL_H_

//------------------- PRE-INCLUDE ENVIRONMENT SETTINGS -------------------

//------------------------------- INCLUDES -------------------------------

//-------------------------- MACROS & TYPEDEFS ---------------------------


//--------------------------- GLOBAL VARIABLES ---------------------------

extern int ctrlPadStatusCurrent;
extern int ctrlPadStatusOld;
extern int ctrlPadStatusNew;
extern int ctrlPadStatusDelta;

//--------------------------- GLOBAL FUNCTIONS ---------------------------

extern void CtrlUpdate( void );
extern void CtrlInit( void );
extern void CtrlShutdown( void );

#endif
