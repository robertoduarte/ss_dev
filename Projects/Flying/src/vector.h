//------------------------------------------------------------------------
//
//	VECTOR3F.H
//  VECTOR3F math header
//
//	CONFIDENTIAL
//	Copyright (c) 1996, Sega Technical Institute
//
//	AUTHOR
//  Russell Bornsch++, 2/96
//
//	TARGET
//	GCC for SH2
//
//	REVISION
//	  2/8/96 - RAB - Initial version
//	  8/7/96 - RAB - Cleanup for demo release
//
//------------------------------------------------------------------------

#ifndef _VECTOR_H_
#define _VECTOR_H_

#include "enigma.h"

extern VECTOR3F vpxu;
extern VECTOR3F vpyu;
extern VECTOR3F vpzu;
extern VECTOR3F vnxu;
extern VECTOR3F vnyu;
extern VECTOR3F vnzu;

extern void VecCopy(VECTOR3F* dest, VECTOR3F* source);
extern void VecAdd(VECTOR3F* dest, VECTOR3F* source);
extern void VecAAdd(VECTOR3A* dest, VECTOR3A* source);
extern void VecSub(VECTOR3F* dest, VECTOR3F* source);
extern void VecNeg(VECTOR3F* dest);

extern FIXED VecMag(VECTOR3F* source);
extern void VecNormalize(VECTOR3F* dest);
extern void VecScale(VECTOR3F* dest, FIXED scale);
extern void VecCross(VECTOR3F* dest, VECTOR3F* a, VECTOR3F* b);
extern FIXED VecDot(VECTOR3F* a, VECTOR3F* b);

#endif  // _VECTOR3F_H_
