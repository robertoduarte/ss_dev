#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H
#include "SL_DEF.H"
#include "Event.h"

// Event Listener function definition
typedef void (*EventListener)(Event *);

void EventManager_Init(short eventCapacity, short eventHandlerCapacity);

void EventManager_Update();

Bool EventManager_QueueEvent(EventType type, unsigned arg);

void EventManager_AbortEvent(EventType type, Bool allOfType);

void EventManager_TriggerEvent(EventType type, unsigned arg);

Bool EventManager_AddListener(EventType type, EventListener eventListener);

Bool EventManager_RemoveListener(EventType type, EventListener eventListener);

#endif /* EVENTMANAGER_H */
