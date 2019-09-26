//------------------------------------------------------------------------
//
//	TREE.H
//	Generic tree structures
//
//	CONFIDENTIAL
//	Copyright (c) 1995-1996, Sega Technical Institute
//
//	AUTHOR
//  Russell Bornsch++, 1/96
//
//	The commented-out portions, which represent an unsuccessful 
//	attempt to implement auto-balancing (AVL) trees, are
//	adapted from Paul Vixie's public domain AVL tree library
//	which was in turn adapted from Niklaus Wirth's book,
//	"Algorithms & Data Structures".
//
//	The unbalanced trees work pretty well, though.
//
//	TARGET
//	GCC for SH2
//
//	REVISION
//	  8/7/96 - RAB - Cleanup for demo release
//
//------------------------------------------------------------------------

//------------------- PRE-INCLUDE ENVIRONMENT SETTINGS -------------------

#include "master.h"

//------------------------------- INCLUDES -------------------------------

#include <stdio.h>
#include <stdlib.h>
#include "assert.h"
#include "tree.h"
#include "list.h"

//-------------------------- MACROS & TYPEDEFS ---------------------------

//--------------------------- STATIC VARIABLES ---------------------------

//static int treeIBalance;
static int (*treeCallback)(Tree**);
static int (*treeCallbackV)(Tree**, long, long*);
static long treeCallbackV1;
static long* treeCallbackV2;
static int treeNodes;
static int treeCurDepth;
static int treeMaxDepth;

//--------------------------- GLOBAL VARIABLES ---------------------------

//--------------------------- STATIC FUNCTIONS ---------------------------

//void _TreeSprout( Tree** tree, Tree* new_node )
//{
//	Tree* p1;
//	Tree* p2;
//
//	if (*tree == NULL)
//	{
//		*tree = new_node;
//		(*tree)->l = NULL;
//		(*tree)->r = NULL;
//		(*tree)->balance = TREE_BALANCE_CENTER;
//		treeIBalance = TRUE;
//		return;
//	}
//	if (new_node->key < (*tree)->key)
//	{
//		// less - sprout left
//		_TreeSprout( &(*tree)->l, new_node );
//		if (treeIBalance == TRUE)
//		{
//			// left branch has grown
//			switch( (*tree)->balance )
//			{
//				case TREE_BALANCE_RIGHT:		// right was longer, balance restored
//					(*tree)->balance = TREE_BALANCE_CENTER;
//					treeIBalance = FALSE;
//					break;
//
//				case TREE_BALANCE_CENTER:		// was balanced, now left longer but ok
//					(*tree)->balance = TREE_BALANCE_LEFT;
//					break;
//
//				case TREE_BALANCE_LEFT:	// left was long, time to rebalance
//					p1 = (*tree)->l;
//					if (p1->balance == TREE_BALANCE_LEFT)		// "single LL"
//					{
//						(*tree)->l = p1->r;
//						p1->r = *tree;
//						(*tree)->balance = TREE_BALANCE_CENTER;
//						*tree = p1;
//					}
//					else						// "double LR"
//					{
//						p2 = p1->r;
//						p1->r = p2->l;
//						p2->l = p1;
//						(*tree)->l = p2->r;
//						p2->r = *tree;
//
//						if (p2->balance == TREE_BALANCE_LEFT)
//							(*tree)->balance = TREE_BALANCE_RIGHT;
//						else
//							(*tree)->balance = TREE_BALANCE_CENTER;
//			
//						if (p2->balance == TREE_BALANCE_RIGHT)
//							p1->balance = TREE_BALANCE_LEFT;
//						else
//							p1->balance = TREE_BALANCE_CENTER;
//
//						*tree = p2;
//					}							// end-else
//					(*tree)->balance = TREE_BALANCE_CENTER;
//					treeIBalance = FALSE;
//					break;
//			}									// end-switch
//		}										// end-if
//		return;
//	}											// end-if
//	else
//	{
//		// more - sprout right
//		_TreeSprout( &(*tree)->r, new_node );
//		if (treeIBalance == TRUE)
//		{
//			// right branch has grown
//			switch( (*tree)->balance )
//			{
//				case TREE_BALANCE_LEFT:	// left was longer, balance restored
//					(*tree)->balance = TREE_BALANCE_CENTER;
//					treeIBalance = FALSE;
//					break;
//
//				case TREE_BALANCE_CENTER:		// was balanced, now right longer but ok
//					(*tree)->balance = TREE_BALANCE_RIGHT;
//					break;
//
//				case 1:		// right was long, time to rebalance
//					p1 = (*tree)->r;
//					if (p1->balance == TREE_BALANCE_RIGHT)		// "single RR"
//					{
//						(*tree)->r = p1->l;
//						p1->l = *tree;
//						(*tree)->balance = TREE_BALANCE_CENTER;
//						*tree = p1;
//					}
//					else						// "double RL"
//					{
//						p2 = p1->l;
//						p1->l = p2->r;
//						p2->r = p1;
//						(*tree)->r = p2->l;
//						p2->l = *tree;
//
//						if (p2->balance == TREE_BALANCE_RIGHT)
//							(*tree)->balance = TREE_BALANCE_LEFT;
//						else
//							(*tree)->balance = TREE_BALANCE_CENTER;
//
//						if (p2->balance == TREE_BALANCE_LEFT)
//							p1->balance = TREE_BALANCE_RIGHT;
//						else
//							p1->balance = TREE_BALANCE_CENTER;
//	
//						*tree = p2;
//					}							// end-else
//					(*tree)->balance = TREE_BALANCE_CENTER;
//					treeIBalance = FALSE;
//					break;
//			}									// end-switch
//		}										// end-if
//	}											// end-else
//}

int _TreeDoIterate(Tree** tree)
{
    if (*tree == NULL)
        return ( CONTINUE);

    if (_TreeDoIterate(&(*tree)->l) == HALT)
        return ( HALT);

    if ((*treeCallback)(tree) == HALT)
        return ( HALT);

    if (_TreeDoIterate(&(*tree)->r) == HALT)
        return ( HALT);

    return ( CONTINUE);
}

int _TreeDoIterateV(Tree** tree)
{
    if (*tree == NULL)
        return ( CONTINUE);

    if (_TreeDoIterateV(&(*tree)->l) == HALT)
        return ( HALT);

    if ((*treeCallbackV)(tree, treeCallbackV1, treeCallbackV2) == HALT)
        return ( HALT);

    if (_TreeDoIterateV(&(*tree)->r) == HALT)
        return ( HALT);

    return ( CONTINUE);
}

void _TreeStatCount(Tree* tree)
{
    treeCurDepth++;

    if (tree->l != NULL)
        _TreeStatCount(tree->l);

    if (treeCurDepth > treeMaxDepth)
        treeMaxDepth = treeCurDepth;

    treeNodes++;

    if (tree->r != NULL)
        _TreeStatCount(tree->r);

    treeCurDepth--;
}

//void _TreeBalanceLeft( Tree** tree )
//{
//	Tree* p1;
//	Tree* p2;
//	
//	switch( (*tree)->balance )
//	{
//		case TREE_BALANCE_LEFT:				// left was longer, now fixed
//		(*tree)->balance = TREE_BALANCE_CENTER;
//		break;
//
//		case 0:					// was balanced, now one off
//		(*tree)->balance = TREE_BALANCE_RIGHT;
//		treeIBalance = FALSE;
//		break;
//
//		case 1:					// was already off, need to balance
//		p1 = (*tree)->r;
//		if (p1->balance >= TREE_BALANCE_CENTER)
//		{
//			// "single RR"
//			(*tree)->r = p1->l;
//			p1->l = *tree;
//			if (p1->balance == TREE_BALANCE_CENTER)
//			{
//				(*tree)->balance = TREE_BALANCE_RIGHT;
//				p1->balance = TREE_BALANCE_LEFT;
//				treeIBalance = FALSE;
//			}
//			else
//			{
//				(*tree)->balance = TREE_BALANCE_CENTER;
//				p1->balance = TREE_BALANCE_CENTER;
//			}
//			*tree = p1;
//		}
//		else
//		{
//			// "double RL"
//			p2 = p1->l;
//			p1->l = p2->r;
//			p2->r = p1;
//			(*tree)->r = p2->l;
//			p2->l = *tree;
//			if (p2->balance == TREE_BALANCE_RIGHT)
//				(*tree)->balance = TREE_BALANCE_LEFT;
//			else
//				(*tree)->balance = TREE_BALANCE_CENTER;
//			if (p2->balance == TREE_BALANCE_LEFT)
//				p1->balance = TREE_BALANCE_RIGHT;
//			else
//				p1->balance = TREE_BALANCE_CENTER;
//			*tree = p2;
//			p2->balance = TREE_BALANCE_CENTER;
//		}
//	}
//}
//
//void _TreeBalanceRight( Tree** tree )
//{
//	Tree* p1;
//	Tree* p2;
//
//	switch( (*tree)->balance )
//	{
//		case TREE_BALANCE_RIGHT:					// right was longer, now fixed
//		(*tree)->balance = TREE_BALANCE_CENTER;
//		break;
//
//		case TREE_BALANCE_CENTER:					// was balanced, now one off
//		(*tree)->balance = TREE_BALANCE_LEFT;
//		treeIBalance = FALSE;
//		break;
//
//		case TREE_BALANCE_LEFT:				// was already off, need to balance
//		p1 = (*tree)->l;
//		if (p1->balance <= TREE_BALANCE_CENTER)
//		{
//			// "single LL"
//			(*tree)->l = p1->r;
//			p1->r = *tree;
//			if (p1->balance == TREE_BALANCE_CENTER)
//			{
//				(*tree)->balance = TREE_BALANCE_LEFT;
//				p1->balance = TREE_BALANCE_RIGHT;
//				treeIBalance = FALSE;
//			}
//			else
//			{
//				(*tree)->balance = TREE_BALANCE_CENTER;
//				p1->balance = TREE_BALANCE_CENTER;
//			}
//			*tree = p1;
//		}
//		else
//		{
//			// "double LR"
//			p2 = p1->r;
//			p1->r = p2->l;
//			p2->l = p1;
//			(*tree)->l = p2->r;
//			p2->r = *tree;
//			if (p2->balance == TREE_BALANCE_LEFT)
//				(*tree)->balance = TREE_BALANCE_RIGHT;
//			else
//				(*tree)->balance = TREE_BALANCE_CENTER;
//			if (p2->balance == TREE_BALANCE_RIGHT)
//				p1->balance = TREE_BALANCE_LEFT;
//			else
//				p1->balance = TREE_BALANCE_CENTER;
//			*tree = p2;
//			p2->balance = TREE_BALANCE_CENTER;
//		}
//	}
//}
//
//void _TreeDeleteHelper( Tree** rtree, Tree** tree )
//{
//	Tree* temp;
//	if ((*rtree)->r != NULL)
//	{
//		_TreeDeleteHelper( &(*rtree)->r, tree );
//		if (treeIBalance == TRUE)
//			_TreeBalanceRight( rtree );
//	}
//	else
//	{
//		temp = *tree;
//		*tree = *rtree;
//		*rtree = (*rtree)->l;
//		// ???
//		(*tree)->l = temp->l;
//		(*tree)->r = temp->r;
//		(*tree)->balance = temp->balance;
//		treeIBalance = TRUE;
//	}
//}
//
//Tree* _TreeDeleteByNode( Tree** tree )
//{
//	Tree* node;
//
//	node = *tree;
//	if (node->r == NULL)
//	{
//		// right subtree null
//		*tree = node->l;
//		treeIBalance = TRUE;
//	}
//	else if (node->l == NULL)
//	{
//		// right subtree non-null, left subtree null
//		*tree = node->r;
//		treeIBalance = TRUE;
//	}
//	else
//	{
//		// neither subtree null
//		_TreeDeleteHelper( &node->l, tree );
//		if (treeIBalance == TRUE)
//			_TreeBalanceLeft( tree );
//	}
//	return( node );
//}
//
//Tree* _TreeDeleteByKey( Tree** tree, long key )
//{
//
//	Tree* ret;
//
//	if (*tree == NULL)
//		return( NULL );
//
//	if ((*tree)->key == key)
//	{
//		return( _TreeDeleteByNode( tree ) );
//	}
//	
//	if ((*tree)->key > key)
//	{
//		// too high - scan left
//		ret = _TreeDeleteByKey( &(*tree)->l, key );
//		if (treeIBalance == TRUE)
//			_TreeBalanceLeft( tree );
//	}
//	else
//	{
//		// too low - scan right
//		ret = _TreeDeleteByKey( &(*tree)->r, key );
//		if (treeIBalance == TRUE)
//			_TreeBalanceRight( tree );
//	}
//		
//	return( ret );	
//}

//--------------------------- GLOBAL FUNCTIONS ---------------------------

void TreeInit(Tree** tree)
{
    *tree = NULL;
}

Tree** TreeSearch(Tree** tree, long key)
{
    if (*tree != NULL)
    {
        if ((*tree)->key == key)
            return ( tree);

        if ((*tree)->key < key)
            return ( TreeSearch(&(*tree)->r, key));
        else
            return ( TreeSearch(&(*tree)->l, key));
    }
    return ( NULL);
}

/* will unbalance the tree! */

void TreeAddQuick(Tree** tree, Tree* new_node)
{
    assert(tree != NULL);

    if (*tree == NULL)
    {
        *tree = new_node;
        (*tree)->l = NULL;
        (*tree)->r = NULL;
        (*tree)->balance = TREE_BALANCE_INVALID;
        return;
    }

    // if we visit a node while quick-changing it, mark things up as much as possible

    (*tree)->balance = TREE_BALANCE_INVALID;

    if ((*tree)->key > new_node->key)
        TreeAddQuick(&(*tree)->l, new_node);
    else
        TreeAddQuick(&(*tree)->r, new_node);
}

/* will unbalance the tree! */

Tree* TreeDeleteByNodeQuick(Tree** node)
{
    Tree* t;
    Tree* x;
    Tree* c;

    assert(node != NULL);
    t = x = *node;

    assert(t != NULL);

    if (t->r == NULL)
        x = x->l;
    else if (t->r->l == NULL)
    {
        x = x->r;
        x->l = t->l;
    }
    else
    {
        c = x->r;
        while (c->l->l != NULL)
            c = c->l;
        x = c->l;
        c->l = x->r;
        x->l = t->l;
        x->r = t->r;
    }
    *node = x;
    return ( t);
}

/* will unbalance the tree! */

Tree* TreeDeleteByKeyQuick(Tree** tree, long key)
{
    Tree** found;

    found = TreeSearch(tree, key);
    if (found == NULL)
        return ( NULL);

    return ( TreeDeleteByNodeQuick(found));
}

//void TreeAdd( Tree** tree, Tree* new_node )
//{
//	treeIBalance = FALSE;
//	_TreeSprout( tree, new_node );
//}

void TreeIterate(Tree** tree, int (*callback)(Tree**))
{
    if (tree == NULL)
        return;

    treeCallback = callback;

    _TreeDoIterate(tree);
}

void TreeIterateValue(Tree** tree, int (*callback)(Tree**, long, long*), long v1, long* v2)
{
    if (tree == NULL)
        return;

    treeCallbackV = callback;
    treeCallbackV1 = v1;
    treeCallbackV2 = v2;

    _TreeDoIterateV(tree);
}

void TreeStats(Tree** tree, int* nodes, int* depth)
{
    treeNodes = 0;
    treeCurDepth = 0;
    treeMaxDepth = 0;

    assert(*tree != NULL);

    _TreeStatCount(*tree);

    *nodes = treeNodes;
    *depth = treeMaxDepth;
}

//Tree* TreeDeleteByKey( Tree** tree, long key )
//{
//	treeIBalance = FALSE;
//	return( _TreeDeleteByKey( tree, key ) );
//}
//
//Tree* TreeDeleteByNode( Tree** node )
//{
//	treeIBalance = FALSE;
//	return( _TreeDeleteByNode( node ) );
//}

