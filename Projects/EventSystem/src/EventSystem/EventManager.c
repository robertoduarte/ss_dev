#include "EventManager.h"
#include "../Utils/list.h"

#define EVENT_QUEUE_COUNT 2
static List *eventQueue[EVENT_QUEUE_COUNT];
static List *eventTypeListenerList[EVENT_TYPE_COUNT];

Event *EventManager_GetNextEvent()
{
    Event *dummyEvent;
    int eventsLeft = 0;

    if (eventsLeft)
    {
        return dummyEvent;
    }
    else
    {
        return 0;
    }
}

EventListener *EventManager_GetNextEventListener(EventType eventType)
{
    EventListener *dummyEventListener;
    int eventListenersLeft = 0;

    if (eventListenersLeft)
    {
        return dummyEventListener;
    }
    else
    {
        return 0;
    }
    LstIterateValue
}

void EventManager_Init()
{
    int i;
    for (i = 0; i < EVENT_QUEUE_COUNT; i++)
    {
        LstInitList(eventQueue[i]);
    }
    for (i = 0; i < EVENT_TYPE_COUNT; i++)
    {
        LstInitList(eventTypeListenerList[i]);
    }
}

void EventManager_Update()
{
    Event *nextEvent;
    while ((nextEvent = EventManager_GetNextEvent()) != 0)
    {

        EventListener *nextEventListener;
        while ((nextEventListener = EventManager_GetNextEventListener(nextEvent->type)) != 0)
        {
            nextEventListener->eventListenerCallback(nextEvent);
        }
    }
}

void EventManager_QueueEvent(Event event)
{
}

void EventManager_AbortEvent(Event event)
{
}

void EventManager_TriggerEvent(Event event)
{
}

void EventManager_AddListener(EventType eventType, EventListenerCallback eventListenerCallback)
{
    EventListener newEventListener = {eventType, eventListenerCallback};
}

void EventManager_RemoveListener(EventListenerCallback eventListenerCallback)
{
}
