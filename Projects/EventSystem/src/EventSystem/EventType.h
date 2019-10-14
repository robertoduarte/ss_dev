#ifndef EVENTTYPE_H
#define EVENTTYPE_H

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

#endif /* EVENTTYPE_H */

