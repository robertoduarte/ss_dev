//------------------------------------------------------------------------
//
//	ASSERT.H
//  Debug assertion declaration
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
//	  8/7/96 - RAB - Added header for demo release
//
//------------------------------------------------------------------------

#undef assert

#ifdef NDEBUG
	#define assert( test )		((void)0)
#else
	void _assert( char* message );

	#define _STR(x)	_VAL(x)
	#define _VAL(x)	#x
	#define assert( test )	( (test) ? ((void)0) : _sassert( #test , "file " __FILE__ ", line " _STR( __LINE__ ) "." ) )

#endif
