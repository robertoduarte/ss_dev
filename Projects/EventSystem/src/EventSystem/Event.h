#ifndef EVENT_H
#define EVENT_H

typedef enum
{
    // Start new games
    E_NEWGAMEEASY,
    E_NEWGAMENORMAL,
    E_NEWGAMEHARD,

    // Game play related
    E_INCREMENTSCORE,
    E_PAUSEGAME,
    E_CONTINUEGAME,
    E_GAMEOVER,

    // Number of event types
    EVENT_TYPE_COUNT
} EventType;

typedef struct
{
    EventType type;
    void *arg;
} Event;

#endif /* EVENT_H */
