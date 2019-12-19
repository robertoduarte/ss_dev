//------------------------------------------------------------------------
//
//	LIST.H
//  Doubly linked list
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
//	 8/28/95 - RAB - Initial
//	  9/8/95 - RAB - More comments added
//	12/18/95 - RAB - Guarded declaration of HALT/CONTINUE
//	  8/7/96 - RAB - Cleanup for demo release
//
//------------------------------------------------------------------------

#ifndef _LIST_H_
#define _LIST_H_

//------------------- PRE-INCLUDE ENVIRONMENT SETTINGS -------------------

//------------------------------- INCLUDES -------------------------------

#include <SL_DEF.H>

//-------------------------- MACROS & TYPEDEFS ---------------------------

// doubly linked and keyed lists

typedef struct _tag_node_
{
    struct _tag_node_ *next; // next node
    struct _tag_node_ *prev; // previous node
    long key;                // sort key
} Node;

// a list uses dummy nodes to avoid special-casing the head and
// tail of the list. It wastes two nodes and two pointers worth
// of memory per list (in this case 32 bytes), but makes the code
// much cleaner.

typedef struct _tag_list_
{
    Node *head;      // pointer to dummy head node
    Node *tail;      // pointer to dummy tail node
    Node dummy_head; // dummy nodes
    Node dummy_tail; // dummy nodes
} List;

// macros to access previous and next nodes from a current node.
// these return NULL if you reach the dummy nodes at head and tail.

#define LstNextNode(list, node) ((node->next == list->tail) ? (NULL) : (node->next))
#define LstPrevNode(list, node) ((node->prev == list->head) ? (NULL) : (node->prev))

#define LstFirstNode(list) (((list->head->next) == list->tail) ? (NULL) : (list->head->next))
#define LstLastNode(list) (((list->tail->prev) == list->head) ? (NULL) : (list->tail->prev))

#ifndef CONTINUE
#define CONTINUE 1
#define HALT 0
#endif

#define LIST_KEY_MAX 0x7FFFFFFF
#define LIST_KEY_MIN 0x00000000

//--------------------------- GLOBAL VARIABLES ---------------------------

//--------------------------- GLOBAL FUNCTIONS ---------------------------

// these all live in LIST.C

void LstInitList(List *list);
void LstAddNodeToHead(List *list, Node *n);
void LstAddNodeToTail(List *list, Node *n);
void LstAddNodeBefore(List *list, Node *n, Node *cur);
void LstAddNodeAfter(List *list, Node *n, Node *cur);
void LstAddNodeByKey(List *list, Node *n);
void LstUnlinkNode(List *list, Node *n);
Node *LstFindNodeByKey(List *list, long key);
void LstIterate(List *list, int (*callback)(Node *n));
void LstIterateValue(List *list, int (*callback)(Node *n, int v1, int *v2), int val1, int *val2);
void LstChangeKey(List *list, Node *n, long key);

#endif
