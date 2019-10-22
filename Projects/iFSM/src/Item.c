
#include "Item.h"

static ObjectHeader item_header = {
    ITEM_TYPE_ID,
    (draw_interface) & item_draw
};

Item * new_Item()
{
    Item* item = Memmalloc(sizeof (item)); //malloc;
    if (item) item_initialize(item);
    return item;
}

void item_initialize(Item * item)
{
    item->header = &item_header;
    item->test_val = 3;
}

void item_draw(Item * item)
{
    if (!is_type(item, ITEM_TYPE_ID)) return;
    slPrint("Draw item", slLocate(10,5));
    slPrintFX(item->test_val, slLocate(0, 2));
}


