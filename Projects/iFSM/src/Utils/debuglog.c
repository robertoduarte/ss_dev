// #include <sgl.h>

#include "debuglog.h"


Debug debugConstructor()
{
    Debug self = Memmalloc(sizeof(struct Debug_Struct));
    // self->set(slLocate(0,0), "FOOOOOOOOOOODA");
    
    // self->get = &getLog;
    // self->set = &setLog;
    // self->length = &get_length;

    slPrint("self->get", slLocate(0,15));
    slPrint("self->set", slLocate(0,16));
    return self;
};

