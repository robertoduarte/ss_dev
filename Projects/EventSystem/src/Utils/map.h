#include "SL_DEF.H"
#include "rbtree.h"

typedef struct Map_s
{
    RedBlackNode *root;
} Map;

void Map_Set(Map map, Uint16 key, void *value);

void *Map_Get(Map map, Uint16 key);

void Map_Remove(Map map, Uint16 key);