#ifndef MAP_H
#define MAP_H

#include "map.h"

void Map_Set(Map map, Uint16 key, void *value)
{
    RedBlackNode *node;
    if (node = Search(map.root, key))
        node->data = value;
    else
        Insert(&map.root, key, value);
}

void *Map_Get(Map map, Uint16 key)
{
    return Search(map.root, key)->data;
}

void Map_Remove(Map map, Uint16 key)
{
    Delete(&map.root, key);
}

#endif // !MAP_H