#include "EventManager.h"
#include "../Utils/Vector.h"

typedef struct
{
    EventType type;
    unsigned int arg;
} Event;

VECT_GENERATE_TYPE(Event);                                      //Creates EventVector type
VECT_GENERATE_TYPE(EventListener);                              //Creates EventListenerVector type
VECT_GENERATE_NAME(EventListenerVector *, EventListenerVector); //Creates EventListenerVectorVector type

static EventVector *g_queues[2] = {NULL, NULL};
static unsigned int g_currentQueue = 0;

static EventListenerVectorVector *g_eventTypeListeners = NULL;

static void CheckQueues()
{
    if (!g_queues[0])
    {
        g_queues[0] = EventVector_Init(1);
        g_queues[1] = EventVector_Init(1);
    }
}

static EventVector *NextQueue()
{
    CheckQueues();
    return g_queues[!g_currentQueue];
}

static EventVector *CurrentQueue()
{
    CheckQueues();
    return g_queues[g_currentQueue];
}

static void SwitchQueues()
{
    g_currentQueue = !g_currentQueue;
}

static void ResetQueue(EventVector *queue)
{
    queue->size = 0;
}

static EventListenerVectorVector *EventTypeListeners()
{
    if (!g_eventTypeListeners)
    {
        g_eventTypeListeners = EventListenerVectorVector_Init(1);
        for (EventType i = 0; i < EventType_Count; i++)
            EventListenerVectorVector_Push(g_eventTypeListeners, EventListenerVector_Init(1));
    }
    return g_eventTypeListeners;
}

void EventManager_TriggerEvent(EventType type, unsigned int arg)
{
    if (type >= EventTypeListeners()->size)
        return;

    EventListenerVector *eventListenerVector = EventListenerVectorVector_At(EventTypeListeners(), type);

    for (int i = 0; i < eventListenerVector->size; i++)
        EventListenerVector_At(eventListenerVector, i)(type, arg);
}

static void Update()
{
    for (int i = 0; i < CurrentQueue()->size; i++)
    {
        Event event = EventVector_At(CurrentQueue(), i);
        EventManager_TriggerEvent(event.type, event.arg);
    }
}

void EventManager_Update()
{
    Update();
    ResetQueue(CurrentQueue());
    SwitchQueues();
}

void EventManager_QueueEvent(EventType type, unsigned int arg)
{
    EventVector_Push(NextQueue(), (Event){type, arg});
}

void EventManager_AddListener(EventType type, EventListener eventListener)
{
    EventListenerVector *eventListenerVector = EventListenerVectorVector_At(EventTypeListeners(), type);
    for (int i = 0; i < eventListenerVector->size; i++)
        if (EventListenerVector_At(eventListenerVector, i) == eventListener)
            return;

    EventListenerVector_Push(eventListenerVector, eventListener);
}

bool EventManager_RemoveListener(EventType type, EventListener eventListener)
{
    EventListenerVector *eventListenerVector = EventListenerVectorVector_At(EventTypeListeners(), type);

    for (int i = 0; i <= eventListenerVector->size; i++)
        if (EventListenerVector_At(eventListenerVector, i) == eventListener)
        {
            EventListenerVector_Remove(eventListenerVector, i);
            return true;
        }

    return false;
}

void EventManager_AbortEvent(EventType type, bool allOfType)
{
    for (int i = NextQueue()->size - 1; i >= 0; i--)
        if (EventVector_At(NextQueue(), i).type == type)
        {
            EventVector_Remove(NextQueue(), i);
            if (!allOfType)
                break;
        }
}
