#include "rbtree.h"
#include "mem_mgr.h"
#include "SL_DEF.H"

typedef enum _Color
{
    BLACK,
    RED
} Color;

inline void SwapColor(RedBlackNode *nodeA, RedBlackNode *nodeB)
{
    Uint16 nodeAColor = nodeA->color;
    nodeA->color = nodeB->color;
    nodeB->color = nodeAColor;
}

inline void LeftRotate(RedBlackNode **rootNode, RedBlackNode *node)
{
    RedBlackNode *rightNode = node->right;

    node->right = rightNode->left;

    if (node->right != NULL)
        node->right->parent = node;

    rightNode->parent = node->parent;

    if (node->parent == NULL)
        *rootNode = rightNode;

    else if (node == node->parent->left)
        node->parent->left = rightNode;
    else
        node->parent->right = rightNode;

    rightNode->left = node;

    node->parent = rightNode;
}

inline void RightRotate(RedBlackNode **rootNode, RedBlackNode *node)
{
    RedBlackNode *leftNode = node->left;

    node->left = leftNode->right;

    if (node->right != NULL)
        node->right->parent = node;

    leftNode->parent = node->parent;

    if (leftNode->parent == NULL)
        *rootNode = leftNode;

    else if (node == node->parent->left)
        node->parent->left = leftNode;
    else
        node->parent->right = leftNode;

    leftNode->right = node;

    node->parent = leftNode;
}

void InsertFixup(RedBlackNode **root, RedBlackNode *node)
{
    RedBlackNode *parent_pt = NULL;
    RedBlackNode *grand_parent_pt = NULL;

    while ((node != root) && (node->color != BLACK) &&
           (node->parent->color == RED))
    {

        parent_pt = node->parent;
        grand_parent_pt = node->parent->parent;

        /*  Case : A 
            Parent of pt is left child of Grand-parent of pt */
        if (parent_pt == grand_parent_pt->left)
        {

            RedBlackNode *uncle_pt = grand_parent_pt->right;

            /* Case : 1 
               The uncle of pt is also red 
               Only Recoloring required */
            if (uncle_pt != NULL && uncle_pt->color == RED)
            {
                grand_parent_pt->color = RED;
                parent_pt->color = BLACK;
                uncle_pt->color = BLACK;
                node = grand_parent_pt;
            }

            else
            {
                /* Case : 2 
                   pt is right child of its parent 
                   Left-rotation required */
                if (node == parent_pt->right)
                {
                    LeftRotate(root, parent_pt);
                    node = parent_pt;
                    parent_pt = node->parent;
                }

                /* Case : 3 
                   pt is left child of its parent 
                   Right-rotation required */
                RightRotate(root, grand_parent_pt);
                SwapColor(parent_pt->color, grand_parent_pt->color);
                node = parent_pt;
            }
        }

        /* Case : B 
           Parent of pt is right child of Grand-parent of pt */
        else
        {
            RedBlackNode *uncle_pt = grand_parent_pt->left;

            /*  Case : 1 
                The uncle of pt is also red 
                Only Recoloring required */
            if ((uncle_pt != NULL) && (uncle_pt->color == RED))
            {
                grand_parent_pt->color = RED;
                parent_pt->color = BLACK;
                uncle_pt->color = BLACK;
                node = grand_parent_pt;
            }
            else
            {
                /* Case : 2 
                   pt is left child of its parent 
                   Right-rotation required */
                if (node == parent_pt->left)
                {
                    RightRotate(root, parent_pt);
                    node = parent_pt;
                    parent_pt = node->parent;
                }

                /* Case : 3 
                   pt is right child of its parent 
                   Left-rotation required */
                LeftRotate(root, grand_parent_pt);
                SwapColor(parent_pt->color, grand_parent_pt->color);
                node = parent_pt;
            }
        }
    }

    (*root)->color = BLACK;
}

inline void AssignParent(RedBlackNode *root, RedBlackNode *node)
{
    RedBlackNode *parent = NULL;
    RedBlackNode *nodeIterator = root;
    while (nodeIterator != NULL)
    {
        parent = nodeIterator;

        if (node->key < nodeIterator->key)
            nodeIterator = nodeIterator->left;
        else
            nodeIterator = nodeIterator->right;
    }

    if (node->key > parent->key)
        parent->right = node;
    else
        parent->left = node;

    node->parent = parent;
}

void Insert(RedBlackNode **root, Uint16 key, void *data)
{
    RedBlackNode *newNode = (RedBlackNode *)Memmalloc(sizeof(RedBlackNode));
    newNode->key = key;
    newNode->data = data;
    newNode->left = newNode->right = newNode->parent = NULL;

    if (*root == NULL)
    {
        newNode->color = BLACK;
        *root = newNode;
    }
    else
    {
        AssignParent(*root, newNode);
        newNode->color = RED;
        InsertFixup(root, newNode);
    }
}

RedBlackNode *Search(RedBlackNode *rootNode, Uint16 key)
{
    if (rootNode == NULL || rootNode->key == key)
        return rootNode;

    if (rootNode->key < key)
        return Search(rootNode->right, key);
    else
        return Search(rootNode->left, key);
}

void DeleteFixup(RedBlackNode **root, RedBlackNode *node)
{
    while (node != *root && node->color == BLACK)
    {
        if (node == node->parent->left)
        {
            RedBlackNode *sibling = node->parent->right;
            if (sibling->color == RED)
            {
                sibling->color = BLACK;
                node->parent->color = RED;
                LeftRotate(root, node->parent);
                sibling = node->parent->right;
            }
            if (sibling->left->color == BLACK && sibling->right->color == BLACK)
            {
                sibling->color = RED;
                node = node->parent;
                continue;
            }
            else if (sibling->right->color == BLACK)
            {
                sibling->left->color = BLACK;
                sibling->color = RED;
                RightRotate(root, sibling);
                sibling = node->parent->right;
            }
            if (sibling->right->color == RED)
            {
                sibling->color = node->parent->color;
                node->parent->color = BLACK;
                sibling->right->color = BLACK;
                LeftRotate(root, node->parent);
                node = *root;
            }
        }
        else
        {
            RedBlackNode *sibling = node->parent->left;
            if (sibling->color == RED)
            {
                sibling->color = BLACK;
                node->parent->color = RED;
                RightRotate(root, node->parent);
                sibling = node->parent->left;
            }
            if (sibling->right->color == BLACK && sibling->left->color == BLACK)
            {
                sibling->color = RED;
                node = node->parent;
                continue;
            }
            else if (sibling->left->color == BLACK)
            {
                sibling->right->color = BLACK;
                sibling->color = RED;
                LeftRotate(root, sibling);
                sibling = node->parent->left;
            }
            if (sibling->left->color == RED)
            {
                sibling->color = node->parent->color;
                node->parent->color = BLACK;
                sibling->left->color = BLACK;
                RightRotate(root, node->parent);
                node = *root;
            }
        }
    }
    node->color = BLACK;
}

void Transplant(RedBlackNode **root, RedBlackNode *target, RedBlackNode *replacement)
{
    if (target->parent == NULL)
    {
        *root = replacement;
    }
    else if (target == target->parent->left)
    {
        target->parent->left = replacement;
    }
    else
        target->parent->right = replacement;
    replacement->parent = target->parent;
}

RedBlackNode *TreeMinimum(RedBlackNode *subTreeRoot)
{
    while (subTreeRoot->left != NULL)
    {
        subTreeRoot = subTreeRoot->left;
    }
    return subTreeRoot;
}

Bool Delete(RedBlackNode **root, Uint16 key)
{
    RedBlackNode *node = Search(*root, key);
    if (node == NULL)
    {
        return FALSE;
    }

    RedBlackNode *x;

    Uint16 originalColor = node->color;

    if (node->left == NULL)
    {
        x = node->right;
        Transplant(root, node, node->right);
    }
    else if (node->right == NULL)
    {
        x = node->left;
        Transplant(root, node, node->left);
    }
    else
    {
        RedBlackNode *minimumNode = TreeMinimum(node->right);
        originalColor = minimumNode->color;
        x = minimumNode->right;
        if (minimumNode->parent == node)
            x->parent = minimumNode;
        else
        {
            Transplant(root, minimumNode, minimumNode->right);
            minimumNode->right = node->right;
            minimumNode->right->parent = minimumNode;
        }
        Transplant(root, node, minimumNode);
        minimumNode->left = node->left;
        minimumNode->left->parent = minimumNode;
        minimumNode->color = node->color;
    }
    Memfree(node);

    if (originalColor == BLACK)
        DeleteFixup(root, x);
    return TRUE;
}