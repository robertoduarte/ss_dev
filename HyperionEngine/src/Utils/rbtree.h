#pragma once

#include <stdbool.h>
#include <stdlib.h>

typedef enum Color
{
    Color_Black,
    Color_red
} Color;

struct RBNode
{
    int key;
    Color color;
    void *data;
    struct RBNode *left, *right, *parent;
};

typedef struct RBNode RBNode;

void RBTree_Insert(RBNode** tree, int key, void *data);

void *RBTree_AllocAndInsert(RBNode **tree, int key, size_t size);

void *RBTree_Search(RBNode *tree, int key);

bool RBTree_Delete(RBNode **tree, int key);

bool RBTree_FreeAndDelete(RBNode **tree, int key);

