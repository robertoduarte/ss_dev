#ifndef RBTREE_H
#define RBTREE_H

#include "SL_DEF.H"

typedef struct _RBNode
{
    Uint16 key;
    Uint16 color;
    void *data;
    struct _RBNode *left, *right, *parent;
} RedBlackNode;

void Insert(RedBlackNode **root, Uint16 key, void *data);

void *GetData(RedBlackNode *root, Uint16 key);

Bool Delete(RedBlackNode **root, Uint16 key);

#endif // !RBTREE_H
