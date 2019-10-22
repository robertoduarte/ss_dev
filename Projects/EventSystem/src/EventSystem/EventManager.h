#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

#include "Event.h"
/*-----------------EventListener Interface-------------------*/
typedef void (*EventListenerCallback)(Event *);

void EventManager_Init();

void EventManager_Update();

void EventManager_QueueEvent(EventType eventType, void *args);

void EventManager_AbortEvent(EventType eventType, void *args);

void EventManager_TriggerEvent(EventType eventType, void *args);

void EventManager_AddListener(EventType eventType, EventListenerCallback eventListenerCallback);

void EventManager_RemoveListener(EventType eventType, EventListenerCallback eventListenerCallback);

#endif /* EVENTMANAGER_H */
