#ifndef RBTREE_H
#define RBTREE_H

#include "SL_DEF.H"

typedef struct _RBNode
{
    unsigned short key;
    unsigned short color;
    void *data;
    struct _RBNode *left, *right, *parent;
} RedBlackNode;

void Insert(RedBlackNode **root, unsigned short key, void *data);

RedBlackNode *Search(RedBlackNode *rootNode, unsigned short key);

Bool Delete(RedBlackNode **root, unsigned short key);

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
