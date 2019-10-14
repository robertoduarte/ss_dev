#ifndef EVENT_H
#define EVENT_H

#include "EventType.h"

typedef struct
{
    EventType type;
    void* arg;
} Event;

#endif /* EVENT_H */

