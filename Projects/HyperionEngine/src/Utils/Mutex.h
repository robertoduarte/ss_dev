#pragma once

#include <stdbool.h>

#define SAFE(type, variable) (*(volatile type *)((type)&variable + 0x20000000))

typedef struct
{
    bool flag[2];
    bool turn;
} Mutex;

void Mutex_Lock(Mutex *mutex, bool isMaster);

void Mutex_Unlock(Mutex *mutex, bool isMaster);
