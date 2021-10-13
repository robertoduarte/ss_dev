#pragma once

#include "ActionEvent.h"

#define AS_EVENT(name) Event_##name,

#define MOVEMENT(_) \
    _(TurnDown)     \
    _(TurnLeft)     \
    _(TurnRight)

#define ACTION(_) \
    _(Destroy)    \
    _(Create)     \
    _(Change)

typedef enum
{
    MOVEMENT(AS_EVENT)
    ACTION(AS_EVENT)
    Event_Count
} EventT;


typedef enum
{
    Entity_Destroy,
    Action_Event,

    EventType_Count
} EventType;

