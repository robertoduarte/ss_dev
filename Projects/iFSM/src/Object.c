#include "Object.h"

int is_type(Object object, int type)
{
    return (OBJECT_HEADER_CAST(object)->type_id == type);
}

void i_draw(Object this)
{
    if (OBJECT_HEADER_CAST(this)->i_draw != UNUSED_INTERFACE)
        OBJECT_HEADER_CAST(this)->i_draw(this);
}

void i_interaction(Object this, Object other)
{
    if (OBJECT_HEADER_CAST(this)->i_interaction != UNUSED_INTERFACE)
        OBJECT_HEADER_CAST(this)->i_interaction(this, other);
}
