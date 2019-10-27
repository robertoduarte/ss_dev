#ifndef ITEM_H
#define ITEM_H

#define ITEM_TYPE_ID 2

#include <sgl.h>
#include"Object.h"
#include "Item.h"
#include "Utils/mem_mgr.h"

typedef struct
{
    /*------Object Header------*/
    ObjectHeader* header;
    /*-----Object Variables----*/
    FIXED test_val;
} Item;

Item * new_Item();
void item_initialize(Item* item);

void item_draw(Item* item);

#endif /* ITEM_H */

