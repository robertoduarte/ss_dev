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

RedBlackNode *Search(RedBlackNode *rootNode, Uint16 key);

Bool Delete(RedBlackNode **root, Uint16 key);

// void printInorder(RedBlackNode *node)
// {
//     if (node == NULL)
//         return;
//     printInorder(node->left);
//     static int i = 0;
//     slPrintFX((FIXED)node->data, slLocate(0, i));
//     i++;
//     printInorder(node->right);
// }

#endif // !RBTREE_H
