//------------------------------------------------------------------------
//
//	GENERIC.H
//  Generic definitions
//
//	CONFIDENTIAL
//	Copyright (c) 1995, Sega Technical Institute
//
//	AUTHOR
//  Russell Bornsch++, 8/95
//
//	TARGET
//	GCC for SH2
//
//	REVISION
//	 8/28/95 - RAB - Initial creation
//	  8/7/96 - RAB - Cleanup for demo release
//
//------------------------------------------------------------------------

#ifndef _GENERIC_H_
#define _GENERIC_H_

typedef int bool;
typedef char byte;
typedef unsigned char ubyte;
typedef short word;
typedef unsigned short uword;
typedef long dword;
typedef unsigned long udword;
typedef unsigned char uchar;
// GNUSH
//typedef unsigned short	ushort;	
typedef unsigned long ulong;

typedef int BOOL;
typedef char BYTE;
typedef unsigned char UBYTE;
typedef short WORD;
typedef unsigned short UWORD;
typedef long DWORD;
typedef unsigned long UDWORD;
typedef unsigned char UCHAR;
typedef unsigned short USHORT;
typedef unsigned long ULONG;

typedef void* pointer;
typedef void** handle;

#ifndef TRUE
#define  TRUE  1
#define  FALSE  0
#define  true  1
#define  false  0
#endif

#ifndef ON
#define  ON   1
#define  OFF  0
#define  on   1
#define  off  0
#endif

#ifndef YES
#define  YES   1
#define  NO   0
#define  yes   1
#define  no   0
#endif

#ifndef GOOD
#define  GOOD  1
#define  BAD   0
#define  good  1
#define  bad   0
#endif

#ifndef NULL
#define  NULL  (void*)0L
#endif

#endif 



