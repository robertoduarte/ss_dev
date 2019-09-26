//------------------------------------------------------------------------
//
//	COLLIDE.C
//	Enigma actor collisions and attacks
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

#define COLLIDE_MEM_ALLOC(size)			Memmalloc(size)
#define COLLIDE_MEM_FREE(ptr)			Memfree(ptr)

extern EOBJECT* collide_attack_object;
extern EOBJECT* collide_defend_object;
extern VECTOR3F collide_loc;

extern void CollideUpdate( void );
