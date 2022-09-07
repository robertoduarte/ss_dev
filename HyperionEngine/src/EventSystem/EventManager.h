#pragma once

#include <stdbool.h>
#include "EventTypes.h"

// Event Listener function definition
typedef void (*EventListener)(EventType type, unsigned int arg);

void EventManager_Update();

void EventManager_QueueEvent(EventType type, unsigned int arg);

void EventManager_AbortEvent(EventType type, bool allOfType);

void EventManager_TriggerEvent(EventType type, unsigned int arg);

void EventManager_AddListener(EventType type, EventListener eventListener);

bool EventManager_RemoveListener(EventType type, EventListener eventListener);

