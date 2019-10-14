#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

#include "Event.h"
#include "EventListener.h"

void EventManager_Init();

void EventManager_Update();

void EventManager_QueueEvent(Event event);

void EventManager_AbortEvent(Event event);

void EventManager_TriggerEvent(Event event);

void EventManager_AddListener(EventType eventType, EventListenerCallback eventListenerCallback);

void EventManager_RemoveListener(EventListenerCallback eventListenerCallback);

#endif /* EVENTMANAGER_H */
