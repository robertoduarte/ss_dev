#ifndef EVENT_H
#define EVENT_H

typedef enum
{
    // Game Events Description
    ActorMove,        // A game object has moved.
    ActorCollision,   // A collision has occurred.
    AICharacterState, // Character has changed states.
    PlayerState,      // Player has changed states.
    PlayerDeath,      // Player is dead.
    GameOver,         // Player death animation is over.
    ActorCreated,     // A new game object is created.
    ActorDestroy,     // A game object is destroyed.

    // Map/Mission Events
    PreLoadLevel,       // A new level is about to be loaded.
    LoadedLevel,        // A new level is finished loading.
    EnterTriggerVolume, // A character entered a trigger volume.
    ExitTriggerVolume,  // A character exited a trigger volume.
    PlayerTeleported,   // The player has been teleported.

    // Game Startup Events
    GraphicsStarted,      // The graphics system is ready.
    PhysicsStarted,       // The physics system is ready.
    EventSystemStarted,   // The event system is ready.
    SoundSystemStarted,   // The sound system is ready.
    ResourceCacheStarted, // The resource system is ready.
    NetworkStarted,       // The network system is ready.
    HumanViewAttached,    // A human view has been attached.
    GameLogicStarted,     // The game logic system is ready.
    GamePaused,           // The game is paused.
    GameResumedResumed,   // The game is resumed.
    PreSave,              // The game is about to be saved.
    PostSave,             // The game has been saved.

    // Animation and Sound Events
    AnimationStarted,   //An animation has begun.
    AnimationLooped,    //An animation has looped.
    AnimationEnded,     //An animation has ended.
    SoundEffectStarted, //A new sound effect has started.
    SoundEffectLooped,  //A sound effect has looped back to the beginning.
    SoundEffectEnded,   //A sound effect has completed.
    VideoStarted,       //A cinematic has started.
    VideoEnded,         //A cinematic has ended.

    // Entity events
    UpdateEntity,
    RemoveEntity,
    // Test Event
    TestEvent,

    // Number of event types
    EventTypeCount
} EventType;

typedef struct
{
    EventType type;
    unsigned arg;
} Event;

typedef struct
{
    EventType type;
    unsigned arguments[];
} Event2;



#endif /* EVENT_H */
