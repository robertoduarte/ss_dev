#ifndef RBTREE_H
#define RBTREE_H

typedef struct _RBNode
{
    unsigned short key;
    unsigned short color;
    void *data;
    struct _RBNode *left, *right, *parent;
} RedBlackNode;

void Insert(RedBlackNode **root, unsigned short key, void *data);

RedBlackNode *Search(RedBlackNode *rootNode, unsigned short key);

int Delete(RedBlackNode **root, unsigned short key);


#endif // !RBTREE_H
