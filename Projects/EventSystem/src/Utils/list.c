/* ------------------------------------------------------------------------

	LIST.C
 Simple doubly linked list implementation

	CONFIDENTIAL
	Copyright (c) 1995, Sega Technical Institute

	AUTHOR
 Russell Bornsch++, 8/95

	TARGET
	GCC for SH2

	REVISION
	 8/28/95 - RAB - Initial
	10/13/95 - RAB - Catching up on documentation
	  8/7/96 - RAB - Cleanup for demo release

------------------------------------------------------------------------ */

/* ------------------------------ OVERVIEW --------------------------------

	Simple list management module. Provides doubly linked lists with
	insertion sorting based on a longword key, or manual control of
	placement.

------------------------------------------------------------------------ */

//------------------- PRE-INCLUDE ENVIRONMENT SETTINGS -------------------

//#include "master.h"

//------------------------------- INCLUDES -------------------------------

#include <assert.h> // assert
#include "list.h"

//-------------------------- MACROS & TYPEDEFS ---------------------------

//--------------------------- GLOBAL VARIABLES ---------------------------

//--------------------------- STATIC VARIABLES ---------------------------

//--------------------------- STATIC FUNCTIONS ---------------------------

//--------------------------- GLOBAL FUNCTIONS ---------------------------

/* ------------------------------------------------------------------------
	:name:	LstInitList
	:what:	initializes a linked list
	:ins :	pointer to list structure
	:outs:	creates dummy head/tail links
	:uses:
	:note:	unlink all nodes and dispose of them properly before this call
------------------------------------------------------------------------ */

void LstInitList(List *list)
{
    // sanity checks
    assert(list != NULL);

    // we set up pointers so we don't have to type & everywhere later
    list->head = &(list->dummy_head);
    list->tail = &(list->dummy_tail);
    // set up the dummy nodes - head has NO previous, its next is tail
    list->head->prev = NULL;
    list->head->next = list->tail;
    list->head->key = -1;
    // tail has NO next, its prev is head
    list->tail->prev = list->head;
    list->tail->next = NULL;
    list->tail->key = -1;
}

/* ------------------------------------------------------------------------
	:name:	LstAddNodeToHead
	:what:	adds a node to head of the list
	:ins :	pointer to list, pointer to new node
	:outs:	links node into list
	:uses:
	:note:	using this will screw up key-sorted lists
------------------------------------------------------------------------ */

void LstAddNodeToHead(List *list, Node *n)
{
    // sanity checks
    assert(list != NULL);
    assert(n != NULL);

    // adding to head is defined as adding after the dummy head node
    LstAddNodeAfter(list, n, list->head);
}

/* ------------------------------------------------------------------------
	:name:	LstAddNodeToTail
	:what:	adds a node to tail of the list
	:ins :	pointer to list, pointer to new node
	:outs:	links node into list
	:uses:
	:note:	using this will screw up key-sorted lists
------------------------------------------------------------------------ */

void LstAddNodeToTail(List *list, Node *n)
{
    // sanity checks
    assert(list != NULL);
    assert(n != NULL);

    // adding to tail is defined as adding before the dummy tail node
    LstAddNodeBefore(list, n, list->tail);
}

/* ------------------------------------------------------------------------
	:name:	LstAddNodeBefore
	:what:	adds a node before the selected node
	:ins :	pointer to list, pointer to new node, pointer to target node
	:outs:	links node into list
	:uses:
	:note:	using this will screw up key-sorted lists
------------------------------------------------------------------------ */

void LstAddNodeBefore(List *list, Node *n, Node *cur)
{
    // sanity checks
    assert(list != NULL);
    assert(n != NULL);
    assert(cur != NULL);
    assert(cur != list->head);

    // since we have the dummy head/tail nodes,
    // there aren't nasty special cases
    n->prev = cur->prev; // new's previous is current's previous
    n->next = cur;       // new's next is current
    cur->prev->next = n; // current previous next is new
    cur->prev = n;       // current previous is new
}

/* ------------------------------------------------------------------------
	:name:	LstAddNodeAfter
	:what:	adds a node after the selected node
	:ins :	pointer to list, pointer to new node, pointer to target node
	:outs:	links node into list
	:uses:
	:note:	using this will screw up key-sorted lists
------------------------------------------------------------------------ */

void LstAddNodeAfter(List *list, Node *n, Node *cur)
{
    // sanity checks
    assert(list != NULL);
    assert(n != NULL);
    assert(cur != NULL);
    assert(cur != list->tail);

    // since we have the dummy head/tail nodes,
    // there aren't nasty special cases
    n->prev = cur;       // new's previous is current
    n->next = cur->next; // new's next is current's next
    cur->next->prev = n;
    cur->next = n;
}

/* ------------------------------------------------------------------------
	:name:	LstAddNodeByKey
	:what:	inserts a node in key sort order
	:ins :	pointer to list, pointer to new node
	:outs:	links node into list, low nodes first
	:uses:
	:note:	to stay sorted, don't add head/tail, or before/after
------------------------------------------------------------------------ */

void LstAddNodeByKey(List *list, Node *n)
{
    Node *cur;
    register Node *stop = list->tail;
    register long k = n->key;

    // sanity checks
    assert(list != NULL);
    assert(n != NULL);

    // start at top of list
    cur = list->head->next;

    // while there are following nodes
    while (cur != stop)
    {
        // if the following node has higher or same key
        if (cur->key >= k)
        {
            // tuck us in here
            LstAddNodeBefore(list, n, cur);
            return;
        }
        // advance to next node
        cur = cur->next;
    }

    // NO node has higher or same key, so attach at end of list
    LstAddNodeToTail(list, n);
}

/* ------------------------------------------------------------------------
	:name:	LstUnlinkNode
	:what:	takes a node out of the list
	:ins :	pointer to list, pointer to node to extract
	:outs:	unlinks node; node's pointers remain valid until memory freed
	:uses:
	:note:
------------------------------------------------------------------------ */

void LstUnlinkNode(List *list, Node *n)
{
    // sanity checks
    assert(list != NULL);
    assert(n != NULL);
    assert(n != list->head);
    assert(n != list->tail);
    assert(n->prev != NULL);
    assert(n->next != NULL);

    // hook previous and next directly to one another
    // this drops us out of the list
    n->prev->next = n->next;
    n->next->prev = n->prev;
}

/* ------------------------------------------------------------------------
	:name:	LstFindNodeByKey
	:what:	returns a node matching the given key
	:ins :	pointer to list, key to match
	:outs:	pointer to node matching key
	:uses:
	:note:	if multiple nodes have same key, returns one arbitrary such.
------------------------------------------------------------------------ */

Node *LstFindNodeByKey(List *list, long key)
{
    Node *cur;

    // sanity checks
    assert(list != NULL);
    assert(key != -1);

    // start at first valid node
    cur = list->head->next;

    // while there are still valid nodes
    while (cur != list->tail)
    {
        // if we match keys, this is it
        if (cur->key == key)
            return (cur);
        // otherwise, on to next
        cur = cur->next;
    }
    return (NULL);
}

/* ------------------------------------------------------------------------
	:name:	LstIterate
	:what:	Iterate over a list, calling the given function on each node
	:ins :	pointer to list, callback function
	:outs:	calls the callback function on each node
	:uses:
	:note:	stops when any callback returns HALT (0)
------------------------------------------------------------------------ */

void LstIterate(List *list, int (*callback)(Node *n))
{
    Node *cur;
    Node *next;

    // sanity checks
    assert(list != NULL);
    assert(callback != NULL);

    // start scanning from first non-dummy node
    cur = list->head->next;

    // while there are non-dummy nodes following
    while (cur != list->tail)
    {
        // get the next node immediately, because freeing the node
        // under some memory managers will corrupt the data, or we might
        // be moving the current node or something

        next = cur->next;

        // invoke the callback function, stop iterating if NO return

        if ((*callback)(cur) == HALT)
            return;

        // advance to next node

        cur = next;
    }
}

/* ------------------------------------------------------------------------
	:name:	LstIterateValue
	:what:	Iterate over a list, calling the given function on each node
	:ins :	pointer to list, callback function
	:outs:	calls the callback function on each node
	:uses:
	:note:	stops when any callback returns HALT (0)
------------------------------------------------------------------------ */

void LstIterateValue(List *list, int (*callback)(Node *n, Sint32 v1, Sint32 *v2), Sint32 val1, Sint32 *val2)
{
    Node *cur;
    Node *next;

    // sanity checks
    assert(list != NULL);
    assert(callback != NULL);

    // start scanning from first non-dummy node
    cur = list->head->next;

    // while there are non-dummy nodes following
    while (cur != list->tail)
    {
        // get the next node immediately, because freeing the node
        // under some memory managers will corrupt the data, or we might
        // be moving the current node or something

        next = cur->next;

        // invoke the callback function, stop iterating if NO return

        if ((*callback)(cur, val1, val2) == HALT)
            return;

        // advance to next node

        cur = next;
    }
}

/* ------------------------------------------------------------------------
	:name:	LstChangeKey
	:what:	moves an entry in the list according to a change in key
	:ins :	pointer to list, pointer to node to move, new key
	:outs:	changes node's key and puts it in sorted order
	:uses:
	:note:	generally will be faster than unlink+add, not always
------------------------------------------------------------------------ */

void LstChangeKey(List *list, Node *n, long key)
{
    long old_key;
    Node *cur;

    // sanity checks
    assert(list != NULL);
    assert(n != NULL);
    assert(key != -1);

    // copy the old key
    old_key = n->key;

    // NO operation if NO change
    if (old_key == key)
        return;

    // update the key
    n->key = key;

    // the naive strategy here would be to unlink and add-by-key.
    // instead, we'll walk forward or back from the current position,
    // on the assumption that the key will change by a small amount
    // and place the node "nearby" in the list. add-by-key always
    // starts from the head of the list, so this has advantages when the
    // destination position is either near the original position *or*
    // when the destination position is far from the head.

    // unlink it from its current position
    LstUnlinkNode(list, n);

    if (key > old_key)
    {
        // we're advancing in the list - start with next slot
        cur = n->next;
        // then as long as we're on a valid slot
        while (cur != list->tail)
        {
            // if it has greater or same key
            if (cur->key >= n->key)
            {
                // tuck it in
                LstAddNodeBefore(list, n, cur);
                return;
            }
            // otherwise, advance
            cur = cur->next;
        }
        // nothing has a greater key - so attach to tail
        LstAddNodeToTail(list, n);
    }
    else
    {
        // we're retreating in the list - start with prev slot
        cur = n->prev;
        // while there are valid slots
        while (cur != list->head)
        {
            // if this slot has lesser or same key
            if (cur->key <= n->key)
            {
                // tuck it in
                LstAddNodeAfter(list, n, cur);
                return;
            }
            // otherwise, back up
            cur = cur->prev;
        }
        // nothing has a lesser key - so attach to head
        LstAddNodeToHead(list, n);
    }
}
