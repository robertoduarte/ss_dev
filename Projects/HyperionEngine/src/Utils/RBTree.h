#ifndef RBTREE_H
#define RBTREE_H

#include <stdbool.h>

typedef struct RBNode RBNode;

typedef struct RBTree
{
    RBNode *root;
} RBTree;

void RBTree_Insert(RBTree tree, int key, void *data);

void *RBTree_Search(RBTree tree, int key);

bool RBTree_Delete(RBTree tree, int key);

#endif // !RBTREE_H
