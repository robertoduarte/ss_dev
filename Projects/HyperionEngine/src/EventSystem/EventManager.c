#include "EventManager.h"
#include <..\..\sh-coff\include\malloc.h>

#define EVENT_QUEUE_COUNT 2

typedef struct
{
    short capacity;
    short last;
    Event *events;
} EventList;

typedef struct
{
    short capacity;
    short last;
    EventListener *EventHandlers;
} EventHandlerList;

static Bool Initialized = FALSE;
static int currentQueue;
static EventList eventLists[EVENT_QUEUE_COUNT];
static EventHandlerList eventHandlerLists[EventTypeCount];

void EventManager_Init(short eventCapacity, short eventHandlerCapacity)
{
    if (Initialized)
        return;

    currentQueue = 0;

    short capacity = ABS(eventCapacity);
    for (int i = 0; i < EVENT_QUEUE_COUNT; i++)
    {
        eventLists[i].last = -1;
        eventLists[i].capacity = capacity;
        eventLists[i].events = malloc(sizeof(Event) * capacity);
    }

    capacity = ABS(eventHandlerCapacity);
    for (int i = 0; i < EventTypeCount; i++)
    {
        eventHandlerLists[i].last = -1;
        eventHandlerLists[i].capacity = capacity;
        eventHandlerLists[i].EventHandlers = malloc(sizeof(EventListener) * capacity);
    }
    Initialized = TRUE;
    
}

void TriggerEvent(Event *event)
{
    if (event->type >= EventTypeCount)
        return;
    for (int j = 0; j <= eventHandlerLists[event->type].last; j++)
    {
        eventHandlerLists[event->type].EventHandlers[j](event);
    }
}
void EventManager_TriggerEvent(EventType type, unsigned arg)
{
    Event event = {type, arg};
    TriggerEvent(&event);
}

void EventManager_Update()
{
    for (int i = 0; i <= eventLists[currentQueue].last; i++)
    {
        TriggerEvent(&eventLists[currentQueue].events[i]);
    }
    //Switching queue for next frame
    if (currentQueue++ == EVENT_QUEUE_COUNT)
        currentQueue = 0;
}

Bool EventManager_QueueEvent(EventType type, unsigned arg)
{
    EventList *eventList = &eventLists[currentQueue];
    if (eventList->last + 1 < eventList->capacity)
    {
        eventList->last++;
        eventList->events[eventList->last].type = type;
        eventList->events[eventList->last].arg = arg;
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

Bool EventManager_AddListener(EventType type, EventListener eventListener)
{
    EventHandlerList *eventHanlderList = &eventHandlerLists[type];
    if (eventHanlderList->last + 1 < eventHanlderList->capacity)
    {
        eventHanlderList->last++;
        eventHanlderList->EventHandlers[eventHanlderList->last] = eventListener;
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

Bool EventManager_RemoveListener(EventType type, EventListener eventListener)
{
    EventHandlerList *eventHanlderList = &eventHandlerLists[type];
    for (int i = 0; i <= eventHanlderList->last; i++)
    {
        if (eventHanlderList->EventHandlers[i] == eventListener)
        {
            if (i != eventHanlderList->last)
            {
                eventHanlderList->EventHandlers[i] = eventHanlderList->EventHandlers[eventHanlderList->last];
            }
            eventHanlderList->last--;
            return TRUE;
        }
    }
    return FALSE;
}

void EventManager_AbortEvent(EventType type, Bool allOfType)
{
    for (int i = eventLists[currentQueue].last; i >= 0; i--)
    {
        if (eventLists[currentQueue].events[i].type == type)
        {
            eventLists[currentQueue].events[i].type = DisabledEvent;
            if (!allOfType)
                break;
        }
    }
}