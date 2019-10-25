#include "EventManager.h"
#include "../Utils/list.h"
#include "../Utils/mem_mgr.h"

#define EVENT_QUEUE_COUNT 2
static int currentQueue = 0;
static List eventQueue[EVENT_QUEUE_COUNT];
static List eventTypeListenerList[EventTypeCount];

void EventManager_Init()
{
    int i;
    for (i = 0; i < EVENT_QUEUE_COUNT; i++)
    {
        LstInitList(&eventQueue[i]);
    }
    for (i = 0; i < EventTypeCount; i++)
    {
        LstInitList(&eventTypeListenerList[i]);
    }
}

void EventManager_Update()
{
    List *eventQueuePtr = &eventQueue[currentQueue];
    Node *nextEventNode;
    while (nextEventNode = LstFirstNode(eventQueuePtr))
    {
        Event *nextEvent = (Event *)nextEventNode->key;

        List *eventTypeListenerListPtr = &eventTypeListenerList[nextEvent->type];
        Node *nextListenerNode = LstFirstNode(eventTypeListenerListPtr);
        while (nextListenerNode != 0)
        {
            EventListenerCallback nextEventListener = (EventListenerCallback)nextListenerNode->key;
            nextEventListener(nextEvent);
            nextListenerNode = LstNextNode(eventTypeListenerListPtr, nextListenerNode);
        }

        //Removing event node from list freeing its memory
        LstUnlinkNode(&eventQueue[currentQueue], nextEventNode);
        Memfree((void *)nextEventNode->key);
        Memfree((void *)nextEventNode);
    }

    //Switching queue for next frame
    if (currentQueue++ == EVENT_QUEUE_COUNT)
        currentQueue = 0;
}

void EventManager_QueueEvent(EventType eventType, void *args)
{
    Event *event = (Event *)Memmalloc(sizeof(Event));
    event->type = eventType;
    event->arg = args;
    Node *node = (Node *)Memmalloc(sizeof(Node));
    node->key = (long)event;
    LstAddNodeToTail(&eventQueue[currentQueue], node);
}

void EventManager_AbortEvent(EventType eventType, int allOfType)
{
    List *eventQueuePtr = &eventQueue[currentQueue];
    Node *previousEventNode = LstLastNode(eventQueuePtr);
    while (previousEventNode != 0)
    {
        LstUnlinkNode(eventQueuePtr, previousEventNode);
        Event *eventToFree = (Event *)previousEventNode;
        if (!allOfType)
            break;
        previousEventNode = LstPrevNode(eventQueuePtr, previousEventNode);
        Memfree(eventToFree);
    }
}

void EventManager_TriggerEvent(EventType eventType, void *args)
{

    Event event = {eventType, args};

    List *eventTypeListenerListPtr = &eventTypeListenerList[eventType];
    Node *nextListenerNode = LstFirstNode(eventTypeListenerListPtr);
    while (nextListenerNode != 0)
    {
        EventListenerCallback nextEventListener = (EventListenerCallback)nextListenerNode->key;
        nextEventListener(&event);
        nextListenerNode = LstNextNode(eventTypeListenerListPtr, nextListenerNode);
    }
}

void EventManager_AddListener(EventType eventType, EventListenerCallback eventListenerCallback)
{
    Node *node = Memmalloc(sizeof(Node));
    node->key = (long)eventListenerCallback;
    LstAddNodeToTail(&eventTypeListenerList[eventType], node);
}

void EventManager_RemoveListener(EventType eventType, EventListenerCallback eventListenerCallback)
{
    Node *node = LstFindNodeByKey(&eventTypeListenerList[eventType], (long)eventListenerCallback);
    LstUnlinkNode(&eventTypeListenerList[eventType], node);
    Memfree(node);
}
