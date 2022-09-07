#include "RBTree.h"
#include "SatMalloc.h"

static void SwapColor(RBNode *nodeA, RBNode *nodeB)
{
    int nodeAColor = nodeA->color;
    nodeA->color = nodeB->color;
    nodeB->color = nodeAColor;
}

static void LeftRotate(RBNode **rootNode, RBNode *node)
{
    RBNode *rightNode = node->right;

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

static void RightRotate(RBNode **rootNode, RBNode *node)
{
    RBNode *leftNode = node->left;

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

static void InsertFixup(RBNode **root, RBNode *node)
{
    RBNode *parentPtr = NULL;
    RBNode *grandParentPtr = NULL;

    while ((node != *root) && (node->color != Color_Black) &&
           (node->parent->color == Color_red))
    {

        parentPtr = node->parent;
        grandParentPtr = node->parent->parent;

        /*  Case : A 
            Parent is left child of Grand-parent */
        if (parentPtr == grandParentPtr->left)
        {

            RBNode *unclePtr = grandParentPtr->right;

            /* Case : 1 
               The uncle is also red 
               Only Recoloring required */
            if (unclePtr != NULL && unclePtr->color == Color_red)
            {
                grandParentPtr->color = Color_red;
                parentPtr->color = Color_Black;
                unclePtr->color = Color_Black;
                node = grandParentPtr;
            }

            else
            {
                /* Case : 2 
                   pt is right child of its parent 
                   Left-rotation required */
                if (node == parentPtr->right)
                {
                    LeftRotate(root, parentPtr);
                    node = parentPtr;
                    parentPtr = node->parent;
                }

                /* Case : 3 
                   pt is left child of its parent 
                   Right-rotation required */
                RightRotate(root, grandParentPtr);
                SwapColor(parentPtr, grandParentPtr);
                node = parentPtr;
            }
        }

        /* Case : B 
           Parent is right child of Grand-parent */
        else
        {
            RBNode *unclePtr = grandParentPtr->left;

            /*  Case : 1 
                The uncle is also red 
                Only Recoloring required */
            if ((unclePtr != NULL) && (unclePtr->color == Color_red))
            {
                grandParentPtr->color = Color_red;
                parentPtr->color = Color_Black;
                unclePtr->color = Color_Black;
                node = grandParentPtr;
            }
            else
            {
                /* Case : 2 
                   pt is left child of its parent 
                   Right-rotation required */
                if (node == parentPtr->left)
                {
                    RightRotate(root, parentPtr);
                    node = parentPtr;
                    parentPtr = node->parent;
                }

                /* Case : 3 
                   pt is right child of its parent 
                   Left-rotation required */
                LeftRotate(root, grandParentPtr);
                SwapColor(parentPtr, grandParentPtr);
                node = parentPtr;
            }
        }
    }

    (*root)->color = Color_Black;
}

static void AssignParent(RBNode *root, RBNode *node)
{
    RBNode *parent = NULL;
    RBNode *nodeIterator = root;
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

static void DeleteFixup(RBNode **root, RBNode *node)
{
    while (node != *root && node->color == Color_Black)
    {
        if (node == node->parent->left)
        {
            RBNode *sibling = node->parent->right;
            if (sibling->color == Color_red)
            {
                sibling->color = Color_Black;
                node->parent->color = Color_red;
                LeftRotate(root, node->parent);
                sibling = node->parent->right;
            }
            if (sibling->left->color == Color_Black && sibling->right->color == Color_Black)
            {
                sibling->color = Color_red;
                node = node->parent;
                continue;
            }
            else if (sibling->right->color == Color_Black)
            {
                sibling->left->color = Color_Black;
                sibling->color = Color_red;
                RightRotate(root, sibling);
                sibling = node->parent->right;
            }
            if (sibling->right->color == Color_red)
            {
                sibling->color = node->parent->color;
                node->parent->color = Color_Black;
                sibling->right->color = Color_Black;
                LeftRotate(root, node->parent);
                node = *root;
            }
        }
        else
        {
            RBNode *sibling = node->parent->left;
            if (sibling->color == Color_red)
            {
                sibling->color = Color_Black;
                node->parent->color = Color_red;
                RightRotate(root, node->parent);
                sibling = node->parent->left;
            }
            if (sibling->right->color == Color_Black && sibling->left->color == Color_Black)
            {
                sibling->color = Color_red;
                node = node->parent;
                continue;
            }
            else if (sibling->left->color == Color_Black)
            {
                sibling->right->color = Color_Black;
                sibling->color = Color_red;
                LeftRotate(root, sibling);
                sibling = node->parent->left;
            }
            if (sibling->left->color == Color_red)
            {
                sibling->color = node->parent->color;
                node->parent->color = Color_Black;
                sibling->left->color = Color_Black;
                RightRotate(root, node->parent);
                node = *root;
            }
        }
    }
    node->color = Color_Black;
}

static void Transplant(RBNode **root, RBNode *target, RBNode *replacement)
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

static RBNode *TreeMinimum(RBNode *subTreeRoot)
{
    while (subTreeRoot->left != NULL)
    {
        subTreeRoot = subTreeRoot->left;
    }
    return subTreeRoot;
}

static RBNode *Search(RBNode *rootNode, int key)
{
    if (rootNode == NULL || rootNode->key == key)
        return rootNode;

    if (rootNode->key < key)
        return Search(rootNode->right, key);
    else
        return Search(rootNode->left, key);
}

void RBTree_Insert(RBNode **root, int key, void *data)
{
    RBNode *newNode = (RBNode *)malloc(sizeof(RBNode));
    newNode->key = key;
    newNode->data = data;
    newNode->left = newNode->right = newNode->parent = NULL;

    if (*root == NULL)
    {
        newNode->color = Color_Black;
        *root = newNode;
    }
    else
    {
        AssignParent(*root, newNode);
        newNode->color = Color_red;
        InsertFixup(root, newNode);
    }
}

void *RBTree_AllocAndInsert(RBNode **root, int key, size_t size)
{
    void *data = malloc(size);

    if (data)
        RBTree_Insert(root, key, data);

    return data;
}

void *RBTree_Search(RBNode *root, int key)
{
    RBNode *search = Search(root, key);
    return (search) ? search->data : NULL;
}

bool RBTree_Delete(RBNode **root, int key)
{
    RBNode *node = Search(*root, key);
    if (node == NULL)
    {
        return false;
    }

    RBNode *temp;

    int originalColor = node->color;

    if (node->left == NULL)
    {
        temp = node->right;
        Transplant(root, node, node->right);
    }
    else if (node->right == NULL)
    {
        temp = node->left;
        Transplant(root, node, node->left);
    }
    else
    {
        RBNode *minimumNode = TreeMinimum(node->right);
        originalColor = minimumNode->color;
        temp = minimumNode->right;
        if (minimumNode->parent == node)
            temp->parent = minimumNode;
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
    free(node);

    if (originalColor == Color_Black)
        DeleteFixup(root, temp);
    return true;
}

bool RBTree_FreeAndDelete(RBNode **root, int key)
{
    free(RBTree_Search(*root, key));
    return RBTree_Delete(root, key);
}
