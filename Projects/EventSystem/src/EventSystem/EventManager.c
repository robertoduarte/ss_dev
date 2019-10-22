#include "EventManager.h"
#include "../Utils/list.h"
#include "../Utils/mem_mgr.h"

#define EVENT_QUEUE_COUNT 2
static int currentQueue = 0;
static List *eventQueue[EVENT_QUEUE_COUNT];
static List *eventTypeListenerList[EVENT_TYPE_COUNT];

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
    Node *nextEventNode;
    while (nextEventNode = LstFirstNode(eventQueue[currentQueue]))
    {
        Event *nextEvent = (Event *)nextEventNode->key;

        Node *nextListenerNode = LstFirstNode(eventTypeListenerList[nextEvent->type]);
        while ((nextListenerNode = LstNextNode(eventTypeListenerList[nextEvent->type], nextListenerNode)) != 0)
        {
            EventListenerCallback nextEventListener = (EventListenerCallback)nextListenerNode->key;
            nextEventListener(nextEvent);
        }

        //Removing event node from list freeing its memory
        LstUnlinkNode(eventQueue[currentQueue], nextEventNode);
        Memfree(nextEventNode->key);
        Memfree(nextEventNode);
    }

    //Switching queue for next frame
    if (currentQueue++ == EVENT_QUEUE_COUNT)
        currentQueue = 0;
}

void EventManager_QueueEvent(EventType eventType, void *args)
{
    Event *event = Memmalloc(sizeof(Event));
    event->type = eventType;
    event->arg = args;
    Node *node = Memmalloc(sizeof(Node));
    node->key = event;
    LstAddNodeToTail(eventQueue[currentQueue], node);
}

void EventManager_AbortEvent(EventType eventType, void *args)
{
}

void EventManager_TriggerEvent(EventType eventType, void *args)
{
    Event event = {eventType, args};

    Node *nextListenerNode = LstFirstNode(eventTypeListenerList[eventType]);
    while ((nextListenerNode = LstNextNode(eventTypeListenerList[eventType], nextListenerNode)) != 0)
    {
        EventListenerCallback nextEventListener = (EventListenerCallback)nextListenerNode->key;
        nextEventListener(&event);
    }
}

void EventManager_AddListener(EventType eventType, EventListenerCallback eventListenerCallback)
{
    Node *node = Memmalloc(sizeof(Node));
    node->key = eventListenerCallback;

    LstAddNodeToTail(eventTypeListenerList[eventType], node);
}

void EventManager_RemoveListener(EventType eventType, EventListenerCallback eventListenerCallback)
{
    Node *node = LstFindNodeByKey(eventTypeListenerList[eventType], eventListenerCallback);
    LstUnlinkNode(eventTypeListenerList[eventType], node);
    Memfree(node);
}
