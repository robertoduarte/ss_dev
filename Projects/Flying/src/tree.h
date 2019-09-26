//------------------------------------------------------------------------
//
//	TREE.H
//	Generic Tree structures
//
//	CONFIDENTIAL
//	Copyright (c) 1995-1996, Sega Technical Institute
//
//	AUTHOR
//  Russell Bornsch++, 1/96
//
//	TARGET
//	GCC for SH2
//
//	REVISION
//	  8/7/96 - RAB - Cleanup for demo release
//
//------------------------------------------------------------------------

#ifndef _TREE_H_
#define _TREE_H_

//------------------- PRE-INCLUDE ENVIRONMENT SETTINGS -------------------


//------------------------------- INCLUDES -------------------------------

#include "generic.h"

//-------------------------- MACROS & TYPEDEFS ---------------------------

typedef struct _tag_tree_
{
    struct _tag_tree_ *l;
    struct _tag_tree_ *r;
    short balance;
    long key;
} Tree;

#ifndef CONTINUE
#define CONTINUE 1
#define HALT  0
#endif

#define TREE_BALANCE_LEFT  -1
#define TREE_BALANCE_CENTER  0
#define TREE_BALANCE_RIGHT  1
#define TREE_BALANCE_INVALID 2

#define TREE_KEY_MAX 0x7FFFFFFF
#define TREE_KEY_MIN 0x00000000

#define TreeAdd( a, b )    TreeAddQuick( (a), (b) )
#define TreeDeleteByKey( a, b )  TreeDeleteByKeyQuick( (a), (b) )
#define TreeDeleteByNode( a )  TreeDeleteByNodeQuick( (a) )

//--------------------------- GLOBAL VARIABLES ---------------------------

//--------------------------- GLOBAL FUNCTIONS ---------------------------

extern void TreeInit(Tree** tree);
extern Tree** TreeSearch(Tree** tree, long key);
extern void TreeIterate(Tree** tree, int (*callback)(Tree**));
extern void TreeIterateValue(Tree** tree, int (*callback)(Tree**, long, long*), long v1, long* v2);
extern void TreeStats(Tree** tree, int* nodes, int* depth);
extern void TreeAddQuick(Tree** tree, Tree* new_node);
extern Tree* TreeDeleteByNodeQuick(Tree** node);
extern Tree* TreeDeleteByKeyQuick(Tree** tree, long key);

#endif
