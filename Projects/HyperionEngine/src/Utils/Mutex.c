#include "Mutex.h"

void Mutex_Lock(Mutex *mutex, bool isMaster)
{
    SAFE(bool, mutex->flag[isMaster]) = true;
    SAFE(bool, mutex->turn) = !isMaster;
    while ((SAFE(bool, mutex->flag[!isMaster]) == true) &&
           (SAFE(bool, mutex->turn) == !isMaster))
    { /* Busy waiting */}
}

void Mutex_Unlock(Mutex *mutex, bool isMaster)
{
    SAFE(bool, mutex->flag[isMaster]) = false;
}
