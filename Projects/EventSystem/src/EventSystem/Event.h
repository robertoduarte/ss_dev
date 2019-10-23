#ifndef EVENT_H
#define EVENT_H

typedef enum
{
    EVENT_TEST,
    EVENT_TEST2,
    EVENT_TEST3,

    // Number of event types
    EVENT_TYPE_COUNT
} EventType;

typedef struct
{
    EventType type;
    void *arg;
} Event;

#endif /* EVENT_H */
