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

#ifndef _MEM_MGR_H
#define _MEM_MGR_H

#include "assert.h"
#include "SL_DEF.H"

//==============================================================================================================================
//================================================================================================================ CONFIGURATION
// The following constants and flags dictate the behavior of the memory manager.
// The MEM_* values can be previous defined elsewhere to override the given defaults.
// The _MEM_* values should not be used outside of this memory manager
//------------------------------------------------------------------------------------------------------------------------------

#ifndef MEM_ENABLE_BLOCK_COPY // enables some extra block copy functions
#define MEM_ENABLE_BLOCK_COPY YES
#endif

#ifndef MEM_ENABLE_COMPACT // enables memory compaction routines
#define MEM_ENABLE_COMPACT YES
#endif

#ifndef MEM_ENABLE_DEBUG // enables some debugging aids
#define MEM_ENABLE_DEBUG YES
#endif

#ifndef MEM_ENABLE_INFO_FULL     // enables information reporting routines not controlled
#define MEM_ENABLE_INFO_FULL YES //		by MEM_ENABLE_INFO_SOME
#endif

#ifndef MEM_ENABLE_INFO_SOME     // enables information reporting routines:
#define MEM_ENABLE_INFO_SOME YES // 		mem_FreeMem, mem_UsedMem
#endif

#ifndef MEM_ENABLE_OWNER_ID // enables ownerID related functions
#define MEM_ENABLE_OWNER_ID YES
#endif

#ifndef MEM_ENABLE_PURGE // enables memory purging functions
#define MEM_ENABLE_PURGE YES
#endif

#ifndef MEM_ENABLE_STDLIB // enables stdlib.h style functions
#define MEM_ENABLE_STDLIB YES
#endif

#ifndef MEM_ENABLE_TRASH // enables "trash" memory functions
#define MEM_ENABLE_TRASH YES
#endif

#ifndef MEM_ENABLE_RELEASE // enables memory release functions
#define MEM_ENABLE_RELEASE YES
#endif

#ifndef MEM_ENABLE_FRA // enables fixed-record allocator
#define MEM_ENABLE_FRA YES
#endif

//..............................................................................................................................
#ifndef MEM_FLAG_ALLOC_HOLES     // allows for small gaps of memory to be included in alloc's
#define MEM_FLAG_ALLOC_HOLES YES // theoretically, this can improve allocation performance
#endif

#ifndef MEM_FLAG_DECLARE_VARS // declares variables -- turn off if you have a global var file
#define MEM_FLAG_DECLARE_VARS YES
#endif

#ifndef MEM_REPLACE_STDLIB // replace existing malloc, calloc, and free functions
#define MEM_REPLACE_STDLIB NO
#endif

//..............................................................................................................................
#ifndef MEM_VAL_HOLE_SIZE // largest block that will be swallowed as a 'hole' during alloc
#define MEM_VAL_HOLE_SIZE 15
#endif

#ifndef MEM_VAL_ID_CHUNK // ownerID's are allocated in blocks of n ID's at a time
#define MEM_VAL_ID_CHUNK 16
#endif

#ifndef MEM_VAL_LARGE_BLOCK     // blocks of this size are larger will use the 'permanent'
#define MEM_VAL_LARGE_BLOCK 256 //		space search method which is better, but slightly slower
#endif                          // 0 forces 'transient'for all; MAXINT force'permanent' for all

#ifndef MEM_VAL_STD_ATTR // attr values which will be used to allocate generic memory
#define MEM_VAL_STD_ATTR (MEM_ATTR_locked + MEM_ATTR_align16)
#endif

#define MEM_MAX_NUM_RELEASES 32

//..............................................................................................................................

#define _MEM_VERSION 0x00090200 // msb to lsb: major ver, minor ver, incremental ver, user ver

#define _MEM_ID_SYSTEM 0  // ownerID for internal memory manager use
#define _MEM_ID_GENERIC 1 // ownerID for generic memory allocation

//==============================================================================================================================
//==================================================================================================================== CONSTANTS

// -- MEMORY BLOCK ATTRIBUTES --
#define MEM_ATTR_locked 0x01 //		block is unmovable, un purgeable
#define MEM_ATTR_fixed 0x02  //		block is unmovable
#define MEM_ATTR_align2 0x04 // 		block must be Sint16 aligned
#define MEM_ATTR_align4 0x08 //					  Sint32
#define MEM_ATTR_align16 0x10
#define MEM_ATTR_perm 0x20         //		force "permanent" block search
#define MEM_ATTR_system 0x40       //		internal use only
#define MEM_ATTR_purgeAtom 0x10000 //		base value for purge levels == purge level 1
#define MEM_ATTR_purgeShift 16
#define MEM_ATTR_purgeMask 0xffff0000
#define MEM_ATTR_LEGAL 0xffff007f

#define _MEM_alignmask1 (-1)   // -- MEMORY ALIGNMENT MASKS
#define _MEM_alignmask2 (-2)   //		to add a new set, add the attr flag (above) and modify
#define _MEM_alignmask4 (-4)   //			_mem_insert_new_memHandle_permanent()
#define _MEM_alignmask16 (-16) //			_mem_insert_new_memHandle_transient()

//==============================================================================================================================
//============================================================================================================ TYPE DECLARATIONS

typedef void *pointer;
typedef void **handle;

#ifndef YES
#define YES 1
#define NO 0
#define yes 1
#define no 0
#endif

#ifndef GOOD
#define GOOD 1
#define BAD 0
#define good 1
#define bad 0
#endif

#ifndef NULL
#define NULL (void *)0L
#endif

typedef struct _memHandle // -- HANDLE RECORD --
{
    pointer memory;        //		the actual allocated memory block
    Sint32 size;           //		size of the allocated block
    Sint32 freeAfter;      //		amount of free space following allocated block
    Sint16 ownerID;        //		identification tag
    Sint16 bucket;         //		a little info for compaction
    Sint32 attributes;     //		attributes defining status of block
    pointer nextMemHandle; //		linked list to rest of memory
    pointer prevMemHandle; //		linked list to rest of memory
    pointer handleBlock;   //		pointer to handle block which owns this mem block
} _memHandle;

typedef struct _memHandleBlock // -- HANDLE BLOCK RECORD --
{
    pointer nextHandleBlock; //		linked list to other blocks
    pointer prevHandleBlock;
    Sint32 freeCount;  //		number of handles in this block that are free
    Sint32 totalCount; //		total number of handles in this block
    //    pointer     mb[0];
    pointer mb[1];
} _memHandleBlock;

//==============================================================================================================================
//========================================================================================================== FUNCTION PROTOTYPES

// -- PRIMARY MEM_MGR FUNCTIONS --
extern void MemInit(pointer heapStart, pointer heapFinish, Sint32 initHandles, Sint32 stepHandles, Sint32 trashSize);
extern handle MemNewHandle(Sint32 size, Sint32 ownerID, Sint32 attributes);
extern handle MemReAllocHandle(Sint32 size, Sint32 ownerID, Sint32 attributes, handle h);
extern void MemDisposeHandle(handle h);
extern Bool MemCheckHandle(handle h);
extern Sint32 MemGetHandleSize(handle h);
extern handle MemSetHandleSize(handle h, Sint32 size);
extern handle MemLock(handle h);
extern handle MemUnlock(handle h);
extern handle MemFindHandle(pointer p);

#if MEM_ENABLE_OWNER_ID
extern void MemDisposeAll(Sint32 ownerID);
extern void MemLockAll(Sint32 ownerID);
extern void MemUnlockAll(Sint32 ownerID);
extern Sint32 MemUniqueID(char *name);
extern Bool MemCheckID(Sint32 ownerID);
#else
#define MemUniqueID(x) 0 //		faux functionality
#define MemCheckID(x) YES
#endif

//..............................................................................................................................

#if MEM_ENABLE_RELEASE
typedef long (*ReleaseFunc)(long, long);
extern void MemRegisterRelease(ReleaseFunc func);
extern void MemUnregisterRelease(ReleaseFunc func);
extern void MemRelease(long size);
#endif

#if MEM_ENABLE_STDLIB // -- STDLIB.H STYLE FUNCTIONS --
extern pointer Memmalloc(Sint32 size);
extern pointer Memcalloc(Sint32 count, Sint32 size);
extern void Memfree(pointer p);
extern handle Memmalloc_h(Sint32 size); //		handle based variations
extern handle Memcalloc_h(Sint32 count, Sint32 size);
extern void Memfree_h(handle h);

#if MEM_REPLACE_STDLIB // -- REPLACE STDLIB.H FUNCTIONS --
#define malloc Memmalloc
#define calloc Memcalloc
#define free Memfree
#endif

#endif

//..............................................................................................................................

#if MEM_ENABLE_PURGE // -- PURGE RELATED FUNCTIONS --
extern handle MemPurgeHandle(handle h);
extern Sint32 MemGetPurge(handle h);
extern Sint32 MemSetPurge(Sint32 purgeLevel, handle h);

#if MEM_ENABLE_OWNER_ID //		ownerID functions
extern void MemPurgeAll(Sint32 ownerID);
extern void MemSetPurgeAll(Sint32 purgeLevel, Sint32 ownerID);
#endif

#endif

//..............................................................................................................................

#if MEM_ENABLE_TRASH // -- TRASH RELATED FUNCTIONS --
extern pointer MemGetTrash(Sint32 size);
extern pointer MemGetTrashAlign(Sint32 size, Sint32 align);
extern Sint32 MemGetTrashSize(void);
extern void MemSetTrashSize(Sint32 trashSize);
#endif

#if MEM_ENABLE_FRA

typedef struct _mem_fra_
{
    int elsize;            // element size
    int num;               // number of elements
    unsigned short *table; // allocate stack space
    unsigned short *stack; // allocate stack pointer
    char *data;            // data buffer
} RecordAllocator;

extern RecordAllocator *MemInitFRA(int elsize, int num);
extern char *MemAllocFRA(RecordAllocator *fra);
extern void MemFreeFRA(RecordAllocator *fra, char *p);
extern void MemShutdownFRA(RecordAllocator *fra);
#endif

#define MemVersion() (_MEM_VERSION)

#if MEM_ENABLE_INFO_SOME || MEM_ENABLE_INFO_FULL // -- PARTIAL INFORMATION REPORTING --
extern Sint32 MemFreeMem(void);
extern Sint32 MemUsedMem(void);
#endif

#if MEM_ENABLE_INFO_FULL // -- FULL INFORMATION REPORTING --
extern Sint32 MemMaxBlock(void);
extern Sint32 MemTotalMem(void);
extern pointer MemStartMem(void);
extern pointer MemEndMem(void);
extern Sint32 MemHandlesInitial(void);
extern Sint32 MemHandlesStep(void);
extern Sint32 MemTotalHandles(void);
extern Sint32 MemFreeHandles(void);
extern Sint32 MemUsedHandles(void);
#endif

//..............................................................................................................................

#if MEM_ENABLE_COMPACT // -- MEMORY COMPACTION RELATED ROUTINES --
extern void MemCompactMem(void);
#endif

//..............................................................................................................................

// -- BLOCK COPY FUNCTIONS --
extern void MemBlockMove(pointer sourcePtr, pointer destPtr, Sint32 count);
#if MEM_ENABLE_BLOCK_COPY
extern void MemPtrToHand(pointer sourcePtr, handle destHandle, Sint32 count);
extern void MemHandToPtr(handle sourceHandle, pointer destPtr, Sint32 count);
extern void MemHandToHand(handle sourceHandle, handle destHandle, Sint32 count);
#endif

//..............................................................................................................................

#if MEM_ENABLE_DEBUG // -- STRICT BULLET-PROOF RELATED ROUTINES --
extern void MemHavoc(void);
extern Bool MemValidate(void);
extern Bool MemCheckATTR(Sint32 attributes);
#else
#define MemCheckATTR(x) YES; //		faux functionality
#endif

//..............................................................................................................................

// -- INTERNAL FUNCTIONS --
extern void _Memattach_memHandle(_memHandle *exist, _memHandle *newbie);
extern pointer _Memcreate_handleBlock(pointer location, Sint32 count, Bool linkflag);
extern void _Memfree_memHandle(_memHandle *block);
extern _memHandle *_Memget_new_memHandle(void);
extern Bool _Meminsert_new_memHandle_permanent(_memHandle *newBlock);
extern Bool _Meminsert_new_memHandle_transient(_memHandle *newBlock);
extern Sint32 _Memsize_handleBlock(Sint32 numhandles);

//==============================================================================================================================
//============================================================================================================= GLOBAL VARIABLES

#if MEM_FLAG_DECLARE_VARS // -- MASTER VARIABLES --
extern pointer memHeapHead;
extern pointer memHeapTail;
extern Sint32 memInitialHandles;
extern Sint32 memStepHandles;
extern Sint32 memFreeHandles;
extern _memHandle *memRootHandle;
extern _memHandle *memLastHandle;
extern _memHandle *memFirstGap;

extern Sint32 memPreCompactMax;
extern Sint32 memPostCompactMax;

#if MEM_ENABLE_OWNER_ID
extern Sint32 memIDs;
extern handle memIDHandle;
extern char **memIDPointer;
#endif

#if MEM_ENABLE_TRASH
extern pointer memTrashStart;
extern pointer memTrashEnd;
extern pointer memTrashCurrent;
extern _memHandle *memTrashHandle;
#endif

#endif

//==============================================================================================================================

#endif

//==================================================================================================================== >> EOF <<
