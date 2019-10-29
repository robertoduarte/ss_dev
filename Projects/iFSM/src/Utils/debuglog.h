//DEBUG LOG
#ifndef DEBUG_H
#define DEBUG_H

#include "mem_mgr.h"

typedef struct Debug_Struct* Debug;

struct Debug_Struct
{
    void (*get)(const void* self);
    char* (*set)(const void* self, char* c);
    int (*length)(const void* self);
};

void getLog(const void* self);
char* setLog(const void* self, char* c);
int (get_length)(const void* self);

#endif
