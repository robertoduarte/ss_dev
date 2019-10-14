#ifndef EVENT_LISTENER_H
#define EVENT_LISTENER_H

#include "Event.h"

/*-----------------EventListener Interface-------------------*/
typedef void (* EventListenerCallback)(Event*);

typedef struct
{
    EventType eventType;
    EventListenerCallback eventListenerCallback;
} EventListener;

#endif /* EVENT_LISTENER_H */

