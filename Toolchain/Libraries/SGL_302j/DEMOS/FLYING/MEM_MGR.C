//------------------------------------------------------------------------
//
//	MEM_MGR.C
//	General purpose memory manager
//
//	CONFIDENTIAL
//	Copyright (c) 1995-1996, Sega Technical Institute
//
//	AUTHOR
//  Robert Morgan, 3/95
//
//	TARGET
//	GCC for SH2, Watcom C/C++ 10.0, MS VC++ 4.0
//
//	REVISION
//	 3/29/95 - RMM - Initial Release
//   11/6/95 - RAB - Naming conventions, FRA, Watcom support
//   11/6/95 - RAB - Release-request mechanism
//  11/29/95 - RAB - Compaction, MemHavoc(), MemValidate()
//	  8/7/96 - RAB - Cleanup for demo release
//
//	This code is fully documented in MEM_MGR.DOC, available wherever
//	fine code is written.
// 
//------------------------------------------------------------------------

#include "master.h"

#include "generic.h"
#include "mem_mgr.h"

#if _TARGET_PC
	#if __WATCOMC__
		#include <i86.h>		// interrupt for DPMI request in MemAutoInit
		#include <stdlib.h>		// system malloc() in MemAutoInit
	#endif

	#ifdef _MSC_VER
		#include <malloc.h>
	#endif
#endif

#include "assert.h"

//==============================================================================================================================

pointer			memHeapHead;
pointer			memHeapTail;
dword			memInitialHandles;
dword			memStepHandles;
dword			memFreeHandles;
_memHandle*		memRootHandle;
_memHandle*		memLastHandle;
_memHandle*		memFirstGap;

#if		MEM_ENABLE_OWNER_ID
dword		memIDs;
handle		memIDHandle;
char**		memIDPointer;
#endif

#if		MEM_ENABLE_TRASH
pointer		memTrashStart;
pointer		memTrashEnd;
pointer		memTrashCurrent;
_memHandle* memTrashHandle;
#endif

static dword memGainable = 0;

#if		MEM_ENABLE_RELEASE
	static ReleaseFunc memReleaseList[ MEM_MAX_NUM_RELEASES ];
	long MemCompactRelease( long, long );
#endif

//=========================================================================================================== FUNCTIONS: PRIMARY

//--------------------------------------------------------------------------------------------------------------------- MemInit
//..............................................................................................................................
void MemInit (pointer heapStart, pointer heapFinish, dword initHandles, dword stepHandles, dword trashSize)
{
#if NDEBUG
	volatile char *stomp;
	volatile char sdata;
	int memok = TRUE;
#endif

#if		MEM_ENABLE_OWNER_ID										// -- VARIABLE SETUP --
	static	char	sysname[] 	=	"MEMORY MANAGER SYSTEM";	//		owner id flags/pointers
	static	char	genname[]	=	"GENERIC";
				    memIDs	=	0;							//		initialize the ownerID list
#elif
			char	sysname;									//		variation for no ownerID function
			char	genericname;
#endif

	assert	(heapStart);										// -- VALIDATE INPUT --
	assert  (heapFinish>=heapStart);
	assert	(((dword)heapFinish-(dword)heapStart)>1024);		//		check for 'reasonable' limits
	assert	(initHandles<1024);
	assert	(stepHandles<1024);
	assert	(stepHandles>3);
	assert	(trashSize<((dword)heapFinish-(dword)heapStart));

#if NDEBUG

	// this does a couple of useful things...
	// it verifies that all memory passed to meminit is writeable
	// and it corrupts all of said memory which will break anything that
	// was counting on access to that memory.

	for (stomp = heapStart; ((stomp <= heapFinish) && memok); stomp++)
	{
		sdata = *stomp;
		*stomp ^= 0xFF;

		if ((sdata^0xFF) != *stomp)
			memok = FALSE;
	}

	assert( memok == TRUE );
#endif

																// -- INIT GLOBAL VARS  --
	memHeapHead		= heapStart;							//		copy input values
	memHeapTail		= heapFinish;
	memInitialHandles	= initHandles;
	memStepHandles	= stepHandles;
	memFreeHandles	= 0;

																// -- CREATE INITIAL HANDLE BLOCK AND ALLOCATE ROOT HANDLE --
	_Memcreate_handleBlock (heapStart, initHandles, 0);		//		initialize handle block at start of memory
	memRootHandle	=	_Memget_new_memHandle();				//		grab the first handle
	memLastHandle =	memRootHandle;						//			and initialize the other pointers
	memFirstGap   =	memRootHandle;
	memRootHandle->memory         =	heapStart;				//		root handle allocates the initial block
	memRootHandle->size           =	_Memsize_handleBlock(initHandles);
	memRootHandle->freeAfter      =	((dword)heapFinish-(dword)heapStart+1) - (memRootHandle->size);
	memRootHandle->attributes     =	MEM_ATTR_locked+MEM_ATTR_fixed+MEM_ATTR_system;
	memRootHandle->nextMemHandle	=	0;
	memRootHandle->prevMemHandle	=	0;
	memRootHandle->ownerID        =	(short)MemUniqueID (sysname);	//		grab owner ID 0 and allocate starting id table
	MemUniqueID (genname);										//		grab owner ID 1 (for generic use)

#if		MEM_ENABLE_TRASH										// -- ALLOCATE STARTING TRASH BUFFER --
	memTrashHandle = (_memHandle*) MemNewHandle(trashSize, _MEM_ID_SYSTEM, MEM_ATTR_fixed+MEM_ATTR_system);
	memTrashStart  = memTrashHandle->memory;
	memTrashCurrent= memTrashStart;
	memTrashEnd    = (pointer) ((dword)memTrashStart + trashSize - 1);
#endif

#if		MEM_ENABLE_RELEASE
	// compaction should be last resort, since previous releases might 
	// improve compaction

	memReleaseList[ MEM_MAX_NUM_RELEASES-1 ] = MemCompactRelease;

#endif
}

//---------------------------------------------------------------------------------------------------------------- MemNewHandle
//..............................................................................................................................

#if MEM_ENABLE_RELEASE
	void MemRegisterRelease( ReleaseFunc func )
	{
		int i;
		assert( func != NULL );

		for (i = 0; i < MEM_MAX_NUM_RELEASES; i++)
		{
			if (memReleaseList[i] == NULL)
			{
				memReleaseList[i] = func;
				return;
			}
		}	
	}

	void MemUnregisterRelease( ReleaseFunc func )
	{
		int i;
		assert( func != NULL );

		for (i = 0; i < MEM_MAX_NUM_RELEASES; i++)
		{
			if (memReleaseList[i] == func)
			{
				memReleaseList[i] = NULL;
				return;
			}
		}	
	}

//..............................................................................................................................
	void MemRelease( long size )
	{
		int i;
		long big;
	
		for (i = 0; i < MEM_MAX_NUM_RELEASES; i++)
		{
			if (memReleaseList[i] != NULL)
			{
				big = (*memReleaseList[i])( size, size+size );
				if (big >= size)
					return;
			}
		}
	}
#endif

//..............................................................................................................................
handle MemNewHandle (dword size, dword ownerID, dword attributes)
{
	_memHandle*	m	=	_Memget_new_memHandle();				// -- VARIABLE SETUP --
	handle result;

	assert	(MemCheckID(ownerID));								// -- VALIDATE INPUT --
	assert	(MemCheckATTR(attributes));

																// -- CONFIGURE HANDLE --
	m->size = size;												//		setup known values
	m->ownerID = (short)ownerID;										//		just a tag value if ownerID disabled
	m->attributes = attributes;
																//		find memory block
	if ( (attributes & MEM_ATTR_perm) || (size >= MEM_VAL_LARGE_BLOCK) )
		result = _Meminsert_new_memHandle_permanent(m) ? (handle) m : 0;
	else
		result = _Meminsert_new_memHandle_transient(m) ? (handle) m : 0;

#if MEM_ENABLE_RELEASE
	// if these failed, release and try again
	if (result == 0)
	{
		MemRelease( size );

		if ( (attributes & MEM_ATTR_perm) || (size >= MEM_VAL_LARGE_BLOCK) )
			result = _Meminsert_new_memHandle_permanent(m) ? (handle) m : 0;
		else
			result = _Meminsert_new_memHandle_transient(m) ? (handle) m : 0;
	}
#endif

	return (result);
}

//------------------------------------------------------------------------------------------------------------ MemReAllocHandle
//..............................................................................................................................
handle MemReAllocHandle (dword size, dword ownerID, dword attributes, handle h)
{
																// -- VARIABLE SETUP --
	#define		hh	((_memHandle*)h)							//		convenience
	_memHandle* p = hh->prevMemHandle;							//		get linked handles
	_memHandle* n = hh->nextMemHandle;
	handle		result;
	pointer		s;

	assert	(MemCheckID(ownerID));								// -- VALIDATE INPUT --
	assert	(MemCheckATTR(attributes));
	assert	(MemCheckHandle(h));
	assert	(p);												// rab 10/31

	s = hh->memory;
																// -- FREE THE HANDLE'S RESOURCES --
	p->freeAfter += hh->size + hh->freeAfter;					//		move space to previous handle
	p->nextMemHandle = n;										//		move links around this handle -- cut this handle out
	if ( n ) n->prevMemHandle = p;

																// -- ADJUST INFO POINTERS --
	if ( hh==memFirstGap )	memFirstGap=p;
	if ( hh==memLastHandle )	memLastHandle=p;

																// -- REALLOCATE THE SPACE FOR THE HANDLE --
	hh->size = size;											//		setup known values
	hh->ownerID = (short)ownerID;
	hh->attributes = attributes;
																//		find memory block
	if ( (attributes & MEM_ATTR_perm) || (size >= MEM_VAL_LARGE_BLOCK) )
		_Meminsert_new_memHandle_permanent(hh);
	else
		_Meminsert_new_memHandle_transient(hh);

	result = hh->memory ? h : 0;									// -- RETURN RESULT BACK --

#if MEM_ENABLE_RELEASE
	// if these failed, release and try again
	if (result == 0)
	{
		MemRelease( size );

		if ( (attributes & MEM_ATTR_perm) || (size >= MEM_VAL_LARGE_BLOCK) )
			_Meminsert_new_memHandle_permanent(hh);
		else
			_Meminsert_new_memHandle_transient(hh);

		result = hh->memory ? h : 0;									// -- RETURN RESULT BACK --
	}
#endif

	if (result)
		MemBlockMove( s, hh->memory, hh->size );

	return ( result );

	#undef		hh
}

//------------------------------------------------------------------------------------------------------------ MemDisposeHandle
//..............................................................................................................................
void MemDisposeHandle (handle h)
{
																// -- VARIABLE SETUP --
	#define		hh	((_memHandle*)h)							//		convenience
	_memHandle* p = hh->prevMemHandle;							//		get linked handles
	_memHandle* n = hh->nextMemHandle;

	assert	(MemCheckHandle(h));								// -- VALIDATE INPUT --

	// add in optimism about compaction
	memGainable += hh->size + hh->freeAfter;
																// -- FREE THE HANDLE'S RESOURCES --
	p->freeAfter += hh->size + hh->freeAfter;					//		move space to previous handle
	p->nextMemHandle = n;										//		move links around this handle -- cut this handle out
	if ( n ) n->prevMemHandle = p;

																// -- ADJUST INFO POINTERS --
	if ( hh==memFirstGap )	memFirstGap=p;
	if ( hh==memLastHandle )	memLastHandle=p;

	_Memfree_memHandle ((_memHandle*) h);						// -- FREE THE HANDLE ITSELF --

	#undef		hh

}

//-------------------------------------------------------------------------------------------------------------- MemCheckHandle
//..............................................................................................................................
bool MemCheckHandle (handle h)
{
	_memHandle*	m = memRootHandle;							// -- VARIABLE SETUP --

	do
	{
		if ( m==(_memHandle*)h ) return YES;					// handle found
		m = m->nextMemHandle;
	} while ( m );

	return NO;													// handle not found -- return error
}

//------------------------------------------------------------------------------------------------------------ MemGetHandleSize
//..............................................................................................................................
dword MemGetHandleSize (handle h)
{
	assert	(MemCheckHandle(h));								// -- VALIDATE INPUT --

	return	( ((_memHandle*)h)->size );
}

//------------------------------------------------------------------------------------------------------------ MemSetHandleSize
//..............................................................................................................................
handle MemSetHandleSize (handle h, dword size)
{
																// -- VARIABLE SETUP --
	#define		hh	((_memHandle*)h)							//		convenience
	dword		total = hh->size + hh->freeAfter;				//		total space available to this handle

	assert	(MemCheckHandle(h));								// -- VALIDATE INPUT --

																// -- ADJUST HANDLE SPACE --
	if ( size <= total )
	{
		hh->size = size;										//		space already exists, adjust values
		hh->freeAfter = total-size;
		#if		MEM_FLAG_ALLOC_HOLES							//		swallow small hole if left here
			if ( hh->freeAfter <= MEM_VAL_HOLE_SIZE )
			{
				hh->size += hh->freeAfter;
				hh->freeAfter=0;
			}
		#endif
	}
	else														//		not enough room, do a full blown alloc
		return MemReAllocHandle (size, hh->ownerID, hh->attributes, h);

	return h;

	#undef		hh
}

//--------------------------------------------------------------------------------------------------------------------- MemLock
//..............................................................................................................................
handle MemLock (handle h)
{
	assert	(MemCheckHandle(h));								// -- VALIDATE INPUT --

	((_memHandle *) h)->attributes |= MEM_ATTR_locked;

	return h;
}

//------------------------------------------------------------------------------------------------------------------- MemUnlock
//..............................................................................................................................
handle MemUnlock (handle h)
{
	assert (MemCheckHandle(h));								// -- VALIDATE INPUT --

	((_memHandle *) h)->attributes &= ~MEM_ATTR_locked;

	return h;
}

//--------------------------------------------------------------------------------------------------------------- MemFindHandle
//..............................................................................................................................
handle MemFindHandle (pointer p)
{
	_memHandle*	m = memRootHandle;							// -- VARIABLE SETUP --

	do
	{
		if ( ((dword)p>=(dword)m->memory) && ((dword)p<((dword)m->memory+m->size) ) ) break;
		m = m->nextMemHandle;
	} while (m);

	return (handle) m;
}

//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
#if	MEM_ENABLE_OWNER_ID

//--------------------------------------------------------------------------------------------------------------- MemDisposeAll
//..............................................................................................................................
void MemDisposeAll (dword ownerID)
{
	_memHandle*	m = memRootHandle;							// -- VARIABLE SETUP --

	assert	(MemCheckID(ownerID));								// -- VALIDATE INPUT --

	do															// -- SCAN THE HANDLES FOR ONES TO DISPOSE --
	{
		if (m->ownerID == (short)ownerID)
			MemDisposeHandle ((handle)m);
		m = m->nextMemHandle;
	} while ( m );

	*(memIDPointer+ownerID) = 0;								// -- FREE OWNER ID --

}

//------------------------------------------------------------------------------------------------------------------ MemLockAll
//..............................................................................................................................
void MemLockAll (dword ownerID)
{
	_memHandle*	m = memRootHandle;							// -- VARIABLE SETUP --

	assert	(MemCheckID(ownerID));								// -- VALIDATE INPUT --

	do
	{
		if ( m->ownerID == (short)ownerID )
			m->attributes |= MEM_ATTR_locked;
		m = m-> nextMemHandle;
	} while ( m );
}

//---------------------------------------------------------------------------------------------------------------- MemUnlockAll
//..............................................................................................................................
void MemUnlockAll (dword ownerID)
{
	_memHandle*	m = memRootHandle;							// -- VARIABLE SETUP --

	assert	(MemCheckID(ownerID));								// -- VALIDATE INPUT --

	do
	{
		if ( m->ownerID == (short)ownerID )
			m->attributes &= ~MEM_ATTR_locked;
		m = m-> nextMemHandle;
	} while ( m );
}

//----------------------------------------------------------------------------------------------------------------- MemUniqueID
//..............................................................................................................................
dword MemUniqueID (char *name)
{
																// -- VARIABLE SETUP --
	dword	clear;												//		counter for clearing
	dword	use;												//		counter for use ID

	if ( !memIDs )											// -- ALLOCATE INITIAL BLOCK --
	{                                                           //		don't have any space yet
		memIDHandle = MemNewHandle (sizeof (char*)*MEM_VAL_ID_CHUNK, _MEM_ID_SYSTEM, MEM_ATTR_system+MEM_ATTR_fixed+MEM_ATTR_align4);
		memIDPointer= *memIDHandle;
		memIDs = MEM_VAL_ID_CHUNK;
		for ( clear=0; clear<MEM_VAL_ID_CHUNK; clear++ )		//		clear table to null
			*(memIDPointer+clear) = 0;
	}

	for ( use=0; use<memIDs; use++ )							// -- FIND FREE BLOCK --
		if ( !(*(memIDPointer+use)) )	break;

	if ( use==memIDs )										// -- NEED TO ENLARGE OWNER ID TABLE --
	{
		memIDs += MEM_VAL_ID_CHUNK;							//		bump id count
		memIDHandle = MemSetHandleSize ((handle)memIDHandle, sizeof (char*) * memIDs);
		memIDPointer= *memIDHandle;
		for ( clear=0; clear<MEM_VAL_ID_CHUNK; clear++ )		//		clear new portion of table to null
			*(memIDPointer+memIDs-clear-1) = 0;
	}

	*(memIDPointer+use) = name;								// -- SET STATUS AND EXIT --

	return use;
}

//------------------------------------------------------------------------------------------------------------------ MemCheckID
//..............................................................................................................................
bool MemCheckID (dword ownerID)
{
	if ( !ownerID ) return YES;									//		0 (system id) is always valid
	if ( ownerID >= memIDs ) return NO;						//		id value too high
	return *(memIDPointer+ownerID) ? YES : NO;				//		return status of id
}

//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------

#endif	// MEM_ENABLE_OWNER_ID

//==============================================================================================================================
//============================================================================================================ FUNCTIONS: STDLIB

#if		MEM_ENABLE_STDLIB

//------------------------------------------------------------------------------------------------------------------- Memmalloc
//..............................................................................................................................
pointer	Memmalloc (dword size)
{
	handle h = Memmalloc_h (size);
	return h ? *h : (pointer) 0;								// error propagation
}

//------------------------------------------------------------------------------------------------------------------- Memcalloc
//..............................................................................................................................
pointer	Memcalloc (dword count, dword size)
{
	handle h = Memcalloc_h (count, size);
	return h ? *h : (pointer) 0;								// error propagation
}

//--------------------------------------------------------------------------------------------------------------------- Memfree
//..............................................................................................................................
void Memfree (pointer p)
{
	MemDisposeHandle (MemFindHandle(p));
}

//----------------------------------------------------------------------------------------------------------------- Memmalloc_h
//..............................................................................................................................
handle Memmalloc_h (dword size)
{
	return	MemNewHandle (size, _MEM_ID_GENERIC, MEM_VAL_STD_ATTR);
}

//----------------------------------------------------------------------------------------------------------------- Memcalloc_h
//..............................................................................................................................
handle Memcalloc_h (dword count, dword size)
{
	handle	h = MemNewHandle (count*size, _MEM_ID_GENERIC, MEM_VAL_STD_ATTR);
	char*   p = (char*) *h;

	if ( h )
		while ( count )
			{
			*(p++) = 0;
			count --;
			}

	return h;
}

//------------------------------------------------------------------------------------------------------------------- Memfree_h
//..............................................................................................................................
void		Memfree_h (handle h)
{
	assert	(MemCheckHandle(h));								// -- VALIDATE INPUT --

	MemDisposeHandle (h);
}

//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------

#endif	// MEM_ENABLE_STDLIB

//==============================================================================================================================
//============================================================================================================= FUNCTIONS: PURGE

#if		MEM_ENABLE_PURGE

//-------------------------------------------------------------------------------------------------------------- MemPurgeHandle
//..............................................................................................................................
handle MemPurgeHandle (handle h)
{
	#define		hh	((_memHandle*)h)							//		convenience

	assert	(MemCheckHandle(h));								// -- VALIDATE INPUT --

																// -- ATTEMPT PURGE --
	if ( hh->attributes & (MEM_ATTR_locked+MEM_ATTR_system) )	//		check for locked / system flags
		return h;												//			cannot purge

	if ( hh->attributes & MEM_ATTR_purgeMask)					//		handle must have a nonzero purge level
	{
		hh->freeAfter += hh->size;								//		to purge, shift alloc'd space to freeAfter
		hh->size = 0;
	}

	return h;

	#undef		hh
}

//----------------------------------------------------------------------------------------------------------------- MemGetPurge
//..............................................................................................................................
dword MemGetPurge (handle h)
{
	assert	(MemCheckHandle(h));								// -- VALIDATE INPUT --

	return ( ((_memHandle*)h)->attributes & MEM_ATTR_purgeMask ) >> MEM_ATTR_purgeShift;
}

//----------------------------------------------------------------------------------------------------------------- MemSetPurge
//..............................................................................................................................
dword MemSetPurge (dword purgeLevel, handle h)
{
	assert	(MemCheckHandle(h));								// -- VALIDATE INPUT --

	((_memHandle *) h)->attributes = ( ((_memHandle *)h)->attributes & ~MEM_ATTR_purgeMask )
								   | ( purgeLevel << MEM_ATTR_purgeShift );
	return purgeLevel;
}

//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------

#if		MEM_ENABLE_OWNER_ID

//----------------------------------------------------------------------------------------------------------------- MemPurgeAll
//..............................................................................................................................
void MemPurgeAll (dword ownerID)
{
	_memHandle*	m = memRootHandle;							// -- VARIABLE SETUP --

	assert	(MemCheckID(ownerID));								// -- VALIDATE INPUT --

	do
	{
		if (m->ownerID == (short)ownerID)
			MemPurgeHandle ((handle)m);
		m=m->nextMemHandle;
	} while ( m );
}

//-------------------------------------------------------------------------------------------------------------- MemSetPurgeAll
//..............................................................................................................................
void MemSetPurgeAll (dword purgeLevel, dword ownerID)
{
	_memHandle*	m = memRootHandle;							// -- VARIABLE SETUP --

	assert	(MemCheckID(ownerID));								// -- VALIDATE INPUT --

	do
	{
		if ( m->ownerID == (short)ownerID )
			m->attributes = (m->attributes & ~MEM_ATTR_purgeMask)
						 | (purgeLevel << MEM_ATTR_purgeShift);
		m = m-> nextMemHandle;
	} while ( m );
}

//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
#endif	//	MEM_ENABLE_OWNER_ID
#endif	//	MEM_ENABLE_PURGE

//==============================================================================================================================
//============================================================================================================= FUNCTIONS: TRASH

#if		MEM_ENABLE_TRASH

//----------------------------------------------------------------------------------------------------------------- MemGetTrash
//..............................................................................................................................
pointer MemGetTrash (dword size)
{
	pointer temp;												// -- VARIABLE SETUP --

																// -- ALLOCATE TRASH
	if ( (dword)memTrashCurrent+size > (dword)memTrashEnd )	//		will overflow -- return start, bump pointer
	{
		memTrashCurrent = (pointer) ((dword)memTrashStart+size);
		return memTrashStart;
	}

	temp = memTrashCurrent;									//		return current, bump pointer
	memTrashCurrent = (pointer) ((dword)memTrashCurrent + size);
	return temp;
}

//------------------------------------------------------------------------------------------------------------ MemGetTrashAlign
//..............................................................................................................................
pointer MemGetTrashAlign (dword size, dword align)
{
	pointer temp;												// -- VARIABLE SETUP --

																// -- ALIGN TRASH POINTER
	memTrashCurrent = (pointer) (((dword)memTrashCurrent + (align-1)) & (-align));

	if ( (dword)memTrashCurrent+size > (dword)memTrashEnd )	//		will overflow -- return aligned start, bump pointer
	{                                                           //		align start
		temp = (pointer) (((dword)memTrashStart + (align-1)) & (-align));
		memTrashCurrent = (pointer) ((dword)temp+size);
		return temp;
	}

	temp = memTrashCurrent;									//		return current, bump pointer
	memTrashCurrent = (pointer) ((dword)memTrashCurrent + size);
	return temp;
}

//------------------------------------------------------------------------------------------------------------- MemGetTrashSize
//..............................................................................................................................
dword MemGetTrashSize (void)
{
	return (dword)memTrashEnd-(dword)memTrashStart+1;
}

//------------------------------------------------------------------------------------------------------------- MemSetTrashSize
//..............................................................................................................................
void MemSetTrashSize (dword trashSize)
{
	memTrashHandle  = (_memHandle*) MemSetHandleSize((handle)memTrashHandle, trashSize);
	if ( memTrashHandle )
	{
		memTrashStart   = memTrashHandle->memory;
		memTrashCurrent = memTrashStart;
		memTrashEnd     = (pointer) ((dword)memTrashStart + trashSize - 1);
	}
	else 
	{
		memTrashStart   = 0;
		memTrashCurrent = 0;
		memTrashEnd     = 0;
	}
}

//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------

#endif	//	MEM_ENABLE_TRASH

//==============================================================================================================================
//============================================================================================================== FUNCTIONS: INFO

#if		MEM_ENABLE_INFO_SOME||MEM_ENABLE_INFO_FULL

//------------------------------------------------------------------------------------------------------------------ MemFreeMem
//..............................................................................................................................
dword MemFreeMem (void)
{
	dword		accum=0;										// -- VARIABLE SETUP --
	_memHandle*	m = memRootHandle;

	do															// -- TALLY UP THE FREE MEMORY --
	{
		accum += m->freeAfter;
		m = m->nextMemHandle;
	} while ( m );

	return accum;
}

//------------------------------------------------------------------------------------------------------------------ MemUsedMem
//..............................................................................................................................
dword MemUsedMem (void)
{
	dword		accum=0;										// -- VARIABLE SETUP --
	_memHandle*	m = memRootHandle;

	do															// -- TALLY UP THE USED MEMORY
	{
		accum += m->size;
		m = m->nextMemHandle;
	} while ( m );

	return accum;
}

//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------

#endif	//	MEM_ENABLE_INFO_SOME || MEM_ENABLE_INFO_FULL
#if		MEM_ENABLE_INFO_FULL

//----------------------------------------------------------------------------------------------------------------- MemMaxBlock
//..............................................................................................................................
dword MemMaxBlock (void)
{
	dword		best=0;											// -- VARIABLE SETUP --
	_memHandle*	m = memRootHandle;

	do															// -- SCAN FOR THE LARGEST FREE SPACE --
	{
		if ( m->freeAfter > best )
			best = m->freeAfter;
		m = m->nextMemHandle;
	} while ( m );

	return best;
}

//----------------------------------------------------------------------------------------------------------------- MemTotalMem
//..............................................................................................................................
dword MemTotalMem (void)
{
	return (dword)memHeapTail - (dword)memHeapHead + 1;
}

//----------------------------------------------------------------------------------------------------------------- MemStartMem
//..............................................................................................................................
pointer	MemStartMem (void)
{
	return memHeapHead;
}

//------------------------------------------------------------------------------------------------------------------- MemEndMem
//..............................................................................................................................
pointer	MemEndMem (void)
{
	return memHeapTail;
}

//----------------------------------------------------------------------------------------------------------- MemHandlesInitial
//..............................................................................................................................
dword MemHandlesInitial (void)
{
	return memInitialHandles;
}

//-------------------------------------------------------------------------------------------------------------- MemHandlesStep
//..............................................................................................................................
dword MemHandlesStep (void)
{
	return memStepHandles;
}

//------------------------------------------------------------------------------------------------------------- MemTotalHandles
//..............................................................................................................................
dword MemTotalHandles (void)
{
	dword				accum	=	0;							// -- VARIABLE SETUP
	_memHandleBlock*	h		=	memHeapHead;

	do															// -- TALLY UP THE HANDLE COUNT --
	{
		accum += h->totalCount;
		h = h->nextHandleBlock;
	} while ( h );

	return accum;
}

//-------------------------------------------------------------------------------------------------------------- MemFreeHandles
//..............................................................................................................................
dword MemFreeHandles (void)
{
	dword				accum	=	0;							// -- VARIABLE SETUP --
	_memHandleBlock*	h		=	memHeapHead;

	do															// -- TALLY UP THE AVAILABLE HANDLES --
	{
		accum += h->freeCount;
		h = h->nextHandleBlock;
	} while ( h );

	return accum;
}

//-------------------------------------------------------------------------------------------------------------- MemUsedHandles
//..............................................................................................................................
dword MemUsedHandles (void)
{
	dword				accum	=	0;							// -- VARIABLE SETUP --
	_memHandleBlock*	h		=	memHeapHead;

	do															// -- TALLY UP THE USED HANDLES --
	{
		accum += (h->totalCount - h->freeCount);
		h = h->nextHandleBlock;
	} while ( h );

	return accum;
}

//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------

#endif	//	MEM_ENABLE_INFO_FULL

//==============================================================================================================================
//=========================================================================================================== FUNCTIONS: COMPACT

#if		MEM_ENABLE_COMPACT

//--------------------------------------------------------------------------------------------------------------- MemCompactMem
//..............................................................................................................................

_memHandle* _MemGetBigHole( _memHandle* butNot )
{
	_memHandle* scan = memRootHandle;
	dword bestSize = 0;
	_memHandle* bestHole = NULL;

	while (scan)
	{
		if (scan != butNot)
		{
			if (scan->freeAfter > bestSize)
			{
				bestSize = scan->freeAfter;
				bestHole = scan;
			}
		}
		scan = scan->nextMemHandle;
	}
	return( bestHole );
}

_memHandle* _MemGetSmallHole( _memHandle* butNot, int bigEnough )
{
	_memHandle* scan = memRootHandle;
	dword bestSize = 0x7FFFFFFF;
	_memHandle* bestHole = NULL;

	while (scan)
	{
		if (scan != butNot)
		{
			if ((scan->freeAfter >= bigEnough) && (scan->freeAfter < bestSize))
			{
				bestSize = scan->freeAfter;
				bestHole = scan;
			}
		}
		scan = scan->nextMemHandle;
	}
	return( bestHole );
}

_memHandle* _MemGetBigBlock( _memHandle* butNot )
{
	_memHandle* scan = memRootHandle;
	dword bestSize = 0;
	_memHandle* bestBlock = NULL;

	while (scan)
	{
		if (scan != butNot)
		{
			if (scan->size > bestSize)
			{
				bestSize = scan->size;
				bestBlock = scan;
			}
		}
		scan = scan->nextMemHandle;
	}
	return( bestBlock );
}

_memHandle* _MemGetSmallBlock( _memHandle* butNot, int bigEnough )
{
	_memHandle* scan = memRootHandle;
	dword bestSize = 0x7FFFFFFF;
	_memHandle* bestBlock = NULL;

	while (scan)
	{
		if (scan != butNot)
		{
			if ((scan->size >= bigEnough) && (scan->size < bestSize))
			{
				bestSize = scan->size;
				bestBlock = scan;
			}
		}
		scan = scan->nextMemHandle;
	}
	return( bestBlock );
}

dword _MemGetAlignMask( dword attr )
{
	if ( attr & MEM_ATTR_align2 ) return _MEM_alignmask2;
	if ( attr & MEM_ATTR_align4 ) return _MEM_alignmask4;
	if ( attr & MEM_ATTR_align16) return _MEM_alignmask16;

	return _MEM_alignmask1;
}

dword _MemGetAlignSize( dword attr )
{
	if ( attr & MEM_ATTR_align2 ) return 2;
	if ( attr & MEM_ATTR_align4 ) return 4;
	if ( attr & MEM_ATTR_align16) return 16;

	return 1;
}

#define MEM_ATTR_nomove		(MEM_ATTR_fixed+MEM_ATTR_locked)

#if MEM_ENABLE_RELEASE

long MemCompactRelease( long reqMax, long reqTotal )
{
	dword poss;

	poss = MemMaxBlock() + memGainable;

	if ( poss >= reqMax )
		MemCompactMem();
	
	poss = MemMaxBlock();
	return( poss );
}

#endif

void		MemCompactMem (void)
{
	_memHandle* m;
	_memHandle* next;
	dword slide;
	pointer dest;

	m = memRootHandle;

	while (m != NULL)
	{
		next = m->nextMemHandle;
		if (next != NULL)
		{
			// if the following handle is movable
			if ((next->attributes & MEM_ATTR_nomove) == 0)
			{
				// if there's enough free space here to profitably slide the following
				if (m->freeAfter >= _MemGetAlignSize( next->attributes ))
				{
					// see how far we can actually slide
					slide = m->freeAfter & _MemGetAlignMask( next->attributes );
					dest = (pointer)(((dword)next->memory) - slide);
					// move the RAM
					MemBlockMove( next->memory, dest, next->size );
					// this block loses that much
					m->freeAfter -= slide;
					// next block gains that much
					next->freeAfter += slide;
					next->memory = dest;
				}
			}
		}

		m = m->nextMemHandle;
	}

	// reset the optimism counter
	memGainable = 0;
}

//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------

#endif	//	MEM_ENABLE_COMPACT

//==============================================================================================================================
//======================================================================================================== FUNCTIONS: BLOCK COPY

//---------------------------------------------------------------------------------------------------------------- MemBlockMove
//..............................................................................................................................
void	MemBlockMove (pointer sourcePtr, pointer destPtr, dword count)
{
																// -- VARIABLE SETUP --
	char*	s=sourcePtr;										//		copy of input pointers
	char*	d=destPtr;

																// -- DO THE COPY --
	if ( s>d )													//	source is above dest -- do forward copy
	{
		while ( count-- )
			*d++ = *s++;
	}
	else														//	dest is above source -- do reverse copy
	{
		while ( count-- )
			*(d+count) = *(s+count);							//	
	}

}

//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------

#if		MEM_ENABLE_BLOCK_COPY

//---------------------------------------------------------------------------------------------------------------- MemPtrToHand
//..............................................................................................................................
void	MemPtrToHand (pointer sourcePtr, handle destHandle, dword count)
{
	assert	(MemCheckHandle(destHandle));						// -- VALIDATE INPUT --
	assert	(MemGetHandleSize(destHandle)>=count);

	MemBlockMove (sourcePtr, *destHandle, count);
}

//--------------------------------------------------------------------------------------------------------------- MemHandToHand
//..............................................................................................................................
void	MemHandToHand (handle sourceHandle, handle destHandle, dword count)
{
	assert	(MemCheckHandle(destHandle));						// -- VALIDATE INPUT --
	assert	(MemCheckHandle(sourceHandle));
	assert	(MemGetHandleSize(destHandle)>=count);
	assert	(MemGetHandleSize(sourceHandle)>=count);

	MemBlockMove (*sourceHandle, *destHandle, count);
}

//---------------------------------------------------------------------------------------------------------------- MemHandToPtr
//..............................................................................................................................
void	MemHandToPtr (handle sourceHandle, pointer destPtr, dword count)
{
	assert	(MemCheckHandle(sourceHandle));					// -- VALIDATE INPUT --
	assert	(MemGetHandleSize(sourceHandle)>=count);

	MemBlockMove (*sourceHandle, destPtr, count);
}

//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------

#endif	//	MEM_ENABLE_BLOCK_COPY

//==============================================================================================================================
//============================================================================================================= FUNCTIONS: DEBUG

#if		MEM_ENABLE_DEBUG

//----------------------------------------------------------------------------------------------------------------- MemValidate
//..............................................................................................................................
bool MemValidate (void)
{
	_memHandle* a;
	_memHandle* b;
	dword end;
	
	a = memRootHandle;
	b = a->nextMemHandle;

	while (a != NULL)
	{
		assert( a->memory );

		if (a->memory != a->memory & _MemGetAlignMask( a->attributes ) )
		{
//			printf("a->memory: %p   required alignment: %x\n", a->memory, _MemGetAlignMask( a->attributes ) );
			return( NO );
		}
		if (b != NULL)
		{
			assert( b->memory );

			end = a->size + a->freeAfter + (dword)a->memory;

			if (end != (dword)b->memory)
			{
//				printf( "a: %p b: %p. a->mem: %p a->free: %x end: %x b->mem: %p\n",
//					a, b, a->memory, end, b->memory );

				return( NO );
			}
			b = b->nextMemHandle;
		}
		a = a->nextMemHandle;
	}
	return( YES );	
}

//-------------------------------------------------------------------------------------------------------------------- MemHavoc

static dword memSeed = 0x1969;

dword _MemRand( void )
{
	memSeed = memSeed * 1103515245L + 12345L;

	memSeed &= 0x7FFFFFFF;

	return( memSeed );
}

dword _MemRandom( dword range )
{
	return( _MemRand() % range );
}

//..............................................................................................................................
void		MemHavoc (void)
{
	_memHandle* a;
	_memHandle* b;
	dword fore;
	dword back;
	pointer dest;
	dword mask;
	
	a = memRootHandle;
	b = a->nextMemHandle;

	while ((a != NULL) && (b != NULL))
	{
		if ((b->attributes & MEM_ATTR_nomove) == 0)
		{
			mask = _MemGetAlignMask( b->attributes );
			// check forward motion space into b's own free block
			fore = b->freeAfter & mask;
			// check backward motion space into a's free block
			back = a->freeAfter & mask;

			if (fore || back)
			{
				if (fore > back)
				{
//					printf("a memory contents: %2X b memory contents: %2X\n", 
//						*(char*)a->memory, *(char*)b->memory );
					// move forwards
					fore = _MemRandom( fore ) + 1;
					fore &= mask;
					if (fore)
					{
						dest = (pointer)(((dword)b->memory) + fore);
//						printf("moving %p to %p, %x bytes\n", 
//							b->memory, dest, b->size );
						MemBlockMove( b->memory, dest, b->size );
						b->memory = dest;
						a->freeAfter += fore;
						b->freeAfter -= fore;

						assert( (long)a->freeAfter >= 0 );
						assert( (long)b->freeAfter >= 0 );
					}
				}
				else
				{
//					printf("a memory contents: %2X b memory contents: %2X\n", 
//						*(char*)a->memory, *(char*)b->memory );
					// move backwards
					back = _MemRandom( back ) + 1;
					back &= mask;
					if (back)
					{
						dest = (pointer)(((dword)b->memory) - back);
//						printf("moving %p to %p, %x bytes\n", 
//							b->memory, dest, b->size );
						MemBlockMove( b->memory, dest, b->size );
						b->memory = dest;
						a->freeAfter -= back;
						b->freeAfter += back;

						assert( (long)a->freeAfter >= 0 );
						assert( (long)b->freeAfter >= 0 );
					}
				}
			}
		}

		a = b;
		b = b->nextMemHandle;
	}
}

//---------------------------------------------------------------------------------------------------------------- MemCheckATTR
//..............................................................................................................................
bool MemCheckATTR (dword attributes)
{
	if ( attributes & ~MEM_ATTR_LEGAL ) return NO;
	return YES;
}

//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------

#endif	//	MEM_ENABLE_DEBUG

//==============================================================================================================================
//========================================================================================================== FUNCTIONS: INTERNAL

//-------------------------------------------------------------------------------------------------------- _Memattach_memHandle
// Attaches a new memory block to the linked list of memory blocks
//		- newbie as attached following exist
//		- Balances sizes and addresses.
//
// in:	exist			-- existing handle to insert after
//		newbie			-- new block to add (attributes, size, memory set correctly)
//------------------------------------------------------------------------------------------------------------------------------

void _Memattach_memHandle (_memHandle* exist, _memHandle* newbie)
{
	_memHandle*	t;												// -- VARIABLE SETUP --

	assert	(MemCheckHandle((handle)exist));					// -- VALIDATE INPUT --
	assert	((dword)exist->memory+exist->size <= (dword)newbie->memory);
	assert	((dword)exist->memory+exist->size+exist->freeAfter >= (dword)newbie->memory+newbie->size);
	assert	(MemCheckATTR(newbie->attributes));
	assert	(MemCheckID(newbie->ownerID));

	newbie->prevMemHandle = exist;								// -- INSERT NEWBIE INTO LIST AFTER EXIST -- //
	t = newbie->nextMemHandle = exist->nextMemHandle;
	if ( t )
		t->prevMemHandle=newbie;
	else
		memLastHandle=newbie;									// -- UPDATE LAST HANDLE VALUE -- //
	exist->nextMemHandle = newbie;

																// -- BALANCE SIZES -- //
	exist->freeAfter = (dword) ((dword)newbie->memory - (dword)exist->memory - exist->size);
	if ( t )
		newbie->freeAfter = (dword)t->memory - (dword)newbie->memory - newbie->size;
	else
		newbie->freeAfter = (dword)memHeapTail+1 - (dword)newbie->memory - newbie->size;

																// -- SWALLOW SMALL HOLES -- //
	#if		MEM_FLAG_ALLOC_HOLES
		if ( newbie->freeAfter <= MEM_VAL_HOLE_SIZE )			//		check newbie for hole
		{
			newbie->size += newbie->freeAfter;
			newbie->freeAfter=0;
		}
		if ( exist->freeAfter <= MEM_VAL_HOLE_SIZE )			//		check exist for hole
		{
			exist->size += exist->freeAfter;
			exist->freeAfter=0;
		}
	#endif


}

//----------------------------------------------------------------------------------------------------- _Memcreate_handleBlock
//Initializes a handle block and attaches it to the list of handle blocks.
//
// in:	location		--	pointer to where handle block should be (space assumed allocated)
//	   	count			--	number of handles for this handle block
//	   	linkflag		--	clear for first call, set for all subsequent call
//..............................................................................................................................
pointer _Memcreate_handleBlock (pointer location, dword count, bool linkflag)
{
																// -- VARIABLE SETUP --
	#define				ll	((_memHandleBlock*) (location))		//		convenience
	_memHandleBlock*	last;
//	pointer				p	= (pointer) ((dword)location + sizeof (_memHandleBlock) + count * sizeof(pointer));

	// _memHandleBlock struct now includes one pointer
	pointer				p	= (pointer) ((dword)location + sizeof (_memHandleBlock) + (count-1) * sizeof(pointer));

	if ( linkflag ) 											// -- SETUP THE LINKS OF HANDLE BLOCKS --
	{
		last = memHeapHead;									//		get last link
		while ( last->nextHandleBlock )
		{
			last=last->nextHandleBlock;
		}
		last->nextHandleBlock = ll;								//		add new link ll to end link t
		ll->prevHandleBlock = last;
	}
	else
		ll->prevHandleBlock = 0;								//		special case for first handle block
	ll->nextHandleBlock = 0;									//		new link ll is now at the end

							                                    // -- INITIALIZE THE HANDLE BLOCK COUNTS
	ll->freeCount	= count;
	ll->totalCount	= count;
	memFreeHandles+=count;									//		update the global free handle counter

	while ( count-- )											// -- CREATE THE TABLE OF HANDLES --
	{
		ll->mb[count] = p;
		((_memHandle*)(ll->mb[count]))->handleBlock = ll;		//		and link back to handle tables from _memHandles
		p = (pointer) ((dword) p + sizeof (_memHandle));
	}

	return location;

	#undef		ll
}

//---------------------------------------------------------------------------------------------------------- _Memfree_memHandle
// Frees a memory block, returning it to the handle block pool
//
// in:	block		-- handle to free
//------------------------------------------------------------------------------------------------------------------------------
void _Memfree_memHandle (_memHandle* block)
{
	_memHandleBlock*	h = block->handleBlock;					// -- VARIABLE SETUP --

	h->mb[h->freeCount++] = block;								// -- FREE THE HANDLE --
	memFreeHandles++;

	// ### IF HANDLE BLOCK IS COMPLETELY FREE, DEALLOCATE BLOCK ?
}

void _Memguarantee_new_memHandle( void )
{
	_memHandleBlock*	h = memHeapHead;							// -- VARIABLE SETUP --
	_memHandle*			m;

	if (memFreeHandles > 1)
		return;

	while ( 1 )													// -- ALLOCATE MEM HANDLE --
	{
		if ( h->freeCount )										//		found a free handle
		{
			memFreeHandles--;
			m = h->mb[--h->freeCount];							// 		-- Use this handle to allocate more handles
			m->size = _Memsize_handleBlock(memStepHandles);
			m->ownerID = _MEM_ID_SYSTEM;
			m->attributes = MEM_ATTR_locked+MEM_ATTR_fixed+MEM_ATTR_system;
			_Meminsert_new_memHandle_permanent(m);				//			find memory block
			assert (m->memory);									//			VALIDATE CREATION, then create new handle block
			_Memcreate_handleBlock(m->memory, memStepHandles, YES);
			return;
		}
		if ( h->nextHandleBlock )								//		go to next block
			h = h->nextHandleBlock;
		else
			assert (h->nextHandleBlock);						//		theoretically unreachable
	}
}

//------------------------------------------------------------------------------------------------------- _Memget_new_memHandle
// Searches the handle blocks for a free mem block.
// If only one is found, a new handle block is allocated to
// 		it and a handle from the new block is returned.
//..............................................................................................................................
_memHandle* _Memget_new_memHandle (void)
{
	_memHandleBlock*	h = memHeapHead;							// -- VARIABLE SETUP --
	_memHandle*			m;


	while ( 1 )													// -- ALLOCATE MEM HANDLE --
	{
		if ( h->freeCount )										//		found a free handle
		{
			memFreeHandles--;									//		update handles available
			if ( memFreeHandles ) 								//		last handle?
				return h->mb[--h->freeCount];					//			no, send it out
			else
			{
				m = h->mb[--h->freeCount];						// 		-- Use this handle to allocate more handles
				m ->size = _Memsize_handleBlock(memStepHandles);
				m->ownerID = _MEM_ID_SYSTEM;
				m->attributes = MEM_ATTR_locked+MEM_ATTR_fixed+MEM_ATTR_system;
				_Meminsert_new_memHandle_permanent(m);			//			find memory block
				assert (m->memory);								//			VALIDATE CREATION, then create new handle block
				_Memcreate_handleBlock(m->memory, memStepHandles, YES);
				return _Memget_new_memHandle();				//			get a new handle from the new block
			}
		}
		if ( h->nextHandleBlock )								//		go to next block
			h = h->nextHandleBlock;
		else
			assert (h->nextHandleBlock);						//		theoretically unreachable
	}
}

//---------------------------------------------------------------------------------------------- _Meminsert_memHandle_permanent
//		Searches memory for the smallest available slot that will handle the request.
//		- Intended for permanent allocation.
//		- If two or more match, the one higher in memory will be used.
//		- The new space is taken from the top of the open space -- this speeds future searches for space.
//		- memLastHandle is adjusted if necessary.
//
// in:	newBlock		-- memory block to add with attributes and size setup
// out:	returns null if error occured
//..............................................................................................................................
bool _Meminsert_new_memHandle_permanent (_memHandle* newBlock)
{
	_memHandle*	m		=	memFirstGap;						// -- VARIABLE SETUP --
	_memHandle* best	=	0;
	dword		bval	=	(dword)memHeapTail - (dword)memHeapHead;
	pointer		temp;
	dword		alignmask = _MEM_alignmask1;

																// -- GENERATE ALIGNMENT MASK --
	if ( (newBlock->attributes) & MEM_ATTR_align2 ) alignmask = _MEM_alignmask2;
	if ( (newBlock->attributes) & MEM_ATTR_align4 ) alignmask = _MEM_alignmask4;
	if ( (newBlock->attributes) & MEM_ATTR_align16) alignmask = _MEM_alignmask16;

	newBlock->memory = 0;										// -- ALLOW FOR SECONDARY TYPE OF ERROR CHECKING --

	do															// -- FIND A SUITABLE BLOCK SPACE --
	{
																//		must have enough space to begin with and the best yet
		if ( (m->freeAfter >= newBlock->size) && (m->freeAfter <= bval) )
		{
			temp = (pointer) (
				   ((dword) ((dword)m->memory+m->size+ m->freeAfter) //	get end of free block
				 		- newBlock->size)						//		get beginning of alloc block
				 		& alignmask);							//		align alloc block
			if ( (dword)temp >= ((dword)m->memory+m->size) )	//		ensure that aligned block is still valid
			{
				newBlock->memory = temp;
				best = m;
				bval = m->freeAfter;
			}
		}
		m = m->nextMemHandle;
	} while ( m );

	if ( !best ) return BAD;									// -- UNABLE TO FIND SUITABLE BLOCK --

	_Memattach_memHandle (best, newBlock);						// -- ATTACH newBlock TO LIST --

	return GOOD;
}

//---------------------------------------------------------------------------------------------- _Meminsert_memHandle_transient
// Searches memory for the first available slot that will handle the request.
//		- Intended for fast allocation.
//		- The new space is taken from the top of the open space -- this speeds future searches for space.
//		- memLastHandle is adjusted if necessary.
//
// in:	newBlock		-- memory block to add with attributes and size setup
// out:	returns null if error occured
//..............................................................................................................................
bool _Meminsert_new_memHandle_transient (_memHandle* newBlock)
{
   _memHandle*	m = memFirstGap;								// -- VARIABLE SETUP --
	pointer		temp;
	dword		alignmask = _MEM_alignmask1;

 																// -- GENERATE ALIGNMENT MASK --
	if ( (newBlock->attributes) & MEM_ATTR_align2 ) alignmask = _MEM_alignmask2;
	if ( (newBlock->attributes) & MEM_ATTR_align4 ) alignmask = _MEM_alignmask4;
	if ( (newBlock->attributes) & MEM_ATTR_align16) alignmask = _MEM_alignmask16;

	newBlock->memory = 0;										// -- ALLOW FOR SECONDARY TYPE OF ERROR CHECKING --

	do															// -- FIND A SUITABLE BLOCK SPACE --
	{
		if ( m->freeAfter >= newBlock->size )					//		must have enough space to begin with
		{
			temp = (pointer) (
				   ((dword) ((dword)m->memory+m->size+m->freeAfter)	//	get end of free block
				 		- newBlock->size)						//		get beginning of alloc block
				 		& alignmask);							//		align alloc block
			if ( (dword)temp >= ((dword)m->memory + m->size) ) 	//		ensure that aligned block is still valid
			{
				newBlock->memory = temp;
				break;
			}
		}
		m = m->nextMemHandle;
	} while ( m );

	if ( !m ) return BAD;										// -- UNABLE TO FIND SUITABLE BLOCK --

	_Memattach_memHandle (m, newBlock);						// -- ATTACH newBlock TO LIST --

	return GOOD;
}

//------------------------------------------------------------------------------------------------------- _Memsize_handleBlock
// Calculates the size that a hypothetical handle block will take.
//		- This is used for allocation of handle block memory prior to creating the handle block
//
// in:	numhandles		--	the number of handles the handle block will manage
//..............................................................................................................................
dword _Memsize_handleBlock (dword numhandles)
{
	return		sizeof (_memHandleBlock)						// 		the header
			  + sizeof (pointer)	* numhandles				//		the table of handle blocks
			  + sizeof (_memHandle) * numhandles;				//		the handle blocks themselves
}

//------------------------------------------------------------------------------------------------------- _Memsize_handleBlock

#if MEM_ENABLE_FRA

RecordAllocator* MemInitFRA( int elsize, int num )
{
	RecordAllocator* fra;
	unsigned short index = 0;
	int i;

	assert( elsize <= 1024 );
	assert( num <= 1024 );
	assert( elsize*num <= 65536L );
	
	fra = (RecordAllocator*)Memmalloc( sizeof( RecordAllocator ) );
	if (fra == NULL)
		return( NULL );

	fra->elsize = elsize;
	fra->num = num;

	fra->table = (unsigned short*)Memmalloc( sizeof(unsigned short) * num );
	if (fra->table == NULL)
	{
		Memfree( fra );
		return( NULL );
	}

	for (i = 0; i < num; i++)
	{
		fra->table[i] = index;
		index += elsize;
	}

	fra->stack = fra->table + num;

	fra->data = (char*)Memmalloc( num * elsize );
	if (fra->data == NULL)
	{
		Memfree( fra->table );
		Memfree( fra );
		return( NULL );
	}

	return( fra );
}

char* MemAllocFRA( RecordAllocator* fra )
{
	if (fra->stack == fra->table)
		return( NULL );

	assert( fra != NULL );
	assert( fra->elsize <= 1024 );
	assert( fra->num <= 1024 );

	return( fra->data + *(--fra->stack) );
}

void MemFreeFRA( RecordAllocator* fra, char* p )
{
	assert( fra->stack != fra->table+fra->num );
	assert( fra != NULL );
	assert( fra->elsize <= 1024 );
	assert( fra->num <= 1024 );
	assert( p != NULL );

	*fra->stack++ = p - fra->data;
}

void MemShutdownFRA( RecordAllocator* fra )
{
	assert( fra != NULL );
	assert( fra->elsize <= 1024 );
	assert( fra->num <= 1024 );

	Memfree( fra->data );
	Memfree( fra->table );
	Memfree( fra );
}

#endif	// MEM_ENABLE_FRA

#if _TARGET_PC

#ifdef _MSC_VER
	
	static char* memAutoArena;
	
	dword MemAutoInit( dword max, dword trash_size )
	{
		dword heap_size;
		char* first;
		char* last;
		int tries;
	
		heap_size = max;
	
		for (tries = 1, first = NULL; first == NULL; heap_size -= MEM_PAGE_SIZE, tries++)
			first = (char*)(malloc)( heap_size );
	
		memAutoArena = first;
		last = first + heap_size - 1;
	
		if (trash_size > heap_size >> 2)
			trash_size = heap_size >> 2;
			
		MemInit( first, last, 8, 8, trash_size );	//	(heapStart, heapFinish, initHandles, stepHandles, trashSize)
		
		return( heap_size );
	}
	
	void MemAutoShutdown( void )
	{
		(free)(memAutoArena);
	}

#else

#if __WATCOMC__

	static char* memAutoArena;
	
	dword MemAutoInit( dword max, dword trash_size )
	{
		// this gets all memory from the DOS/4G extender and puts it into
		// our care.
	
		union REGS regs;
		struct SREGS sregs;
		dword heap_size;
		DPMIMemInfo info;
		char* first;
		char* last;
		int tries;
	
		regs.x.eax = 0x00000500;
		sregs.ds = 0;
		sregs.es = FP_SEG( &info );
		regs.x.edi = FP_OFF( &info );
		int386x( 0x31, &regs, &regs, &sregs );
	
		heap_size = info.NumPhysicalPagesFree * MEM_PAGE_SIZE;
	
		if (heap_size > max)
			heap_size = max;
	
		for (tries = 1, first = NULL; first == NULL; heap_size -= MEM_PAGE_SIZE, tries++)
			first = (char*)(malloc)( heap_size );
	
		memAutoArena = first;
		last = first + heap_size - 1;
	
		if (trash_size > heap_size >> 2)
			trash_size = heap_size >> 2;
			
		MemInit( first, last, 8, 8, trash_size );	//	(heapStart, heapFinish, initHandles, stepHandles, trashSize)
		
		return( heap_size );
	}
	
	void MemAutoShutdown( void )
	{
		(free)(memAutoArena);
	}

#endif	// __WATCOMC__

#endif	// _WINDOWS_ or __WATCOMC__

#endif	// _TARGET_PC
