#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

#include "Event.h"
// Event Callback Definition
typedef void (*EventListenerCallback)(Event *);

void EventManager_Init();

void EventManager_Update();

void EventManager_QueueEvent(EventType eventType, unsigned args);

void EventManager_AbortEvent(EventType eventType, int allOfType);

void EventManager_TriggerEvent(EventType eventType, unsigned args);

void EventManager_AddListener(EventType eventType, EventListenerCallback eventListenerCallback);

void EventManager_RemoveListener(EventType eventType, EventListenerCallback eventListenerCallback);

#endif /* EVENTMANAGER_H */
