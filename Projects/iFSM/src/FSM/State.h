#ifndef STATE_H
#define STATE_H

//State types
typedef enum {
    PATROL,
    ATTACK,
    RUNAWAY,

    STATE_COUNT
}StateType;

//Abstract calls
typedef void (* StartCallBack)(int timer);
typedef void (* UpdateCallBack)(int timer);
typedef void (* ExitCallbBack)();

typedef struct{
    StateType stateType;
    
    StartCallBack startCallBack;
    UpdateCallBack updateCallBack;
    ExitCallbBack exitCallbBack;
        
}State;

#endif /*STATE_H*/