#include <sgl.h>

#include "game.h"
#include "Player.h"
// #include "Enemy.h"
// #include "Item.h"
#include "Utils/mem_mgr.h"
#include "Utils/list.h"
// #include "Controller/control.h"

Object g_object_array[4];

typedef enum
{
    INPUT_JUMP,
    INPUT_DUCK,
    INPUT_RUN,

    INPUT_TYPES_COUNT
} ActionType;

typedef struct
{
    Sint16 actionkeyMap[INPUT_TYPES_COUNT];
    void *actionCallbackMap[INPUT_TYPES_COUNT];
} InputMap;

void InputMap_mapAction(InputMap *inputMap, ActionType actionType, Sint16 keyMap, void *actionHandlerCallback)
{
    inputMap->actionkeyMap[actionType] = keyMap;
    inputMap->actionCallbackMap[actionType] = actionHandlerCallback;
}

typedef struct
{
    InputMap inputMap;
} ExamplePLayer;

void jumpHandler()
{
    //not fine
}

void ExamplePlayer_processInput(ExamplePLayer *examplePLayer, Sint16 input)
{
    ActionType actionType = examplePLayer->inputMap.actionkeyMap[input];
    void (*callback)()  = examplePLayer->inputMap.actionCallbackMap[actionType];
    callback();
}

void exampleOfMapping()
{
    ExamplePLayer player1;
    InputMap_mapAction(&player1.inputMap, INPUT_JUMP, PER_DGT_TA, &jumpHandler);
}

void Game_init()
{
    slInitSystem(TV_320x240, NULL, 1);
    slPerspective(DEGtoANG(60.0));

    // use 512K of work RAM for general heap
    // handle set size = 16, trash RAM = 2K
    MemInit((pointer)0x06040000, (pointer)0x060BFFFF, 16, 16, 0x800);

    g_object_array[0] = new_Player();
    g_object_array[1] = new_Enemy();
    g_object_array[2] = new_Item();
    g_object_array[3] = new_Input();
}



// int GetInput(Input i)
// {
//     if (i.gamepadInput == 1)
//     {
//         slPrint(i.gamepadInput, slLocate(0, 15));
//         return i.gamepadInput;
//     }
//     // return i.gamepadInput;
// }

///*/********************DEBUG LOG
 
int Game_run()
{
    int gamepadInput = Smpc_Peripheral[0].data;

    if(!(gamepadInput & PER_DGT_ST))
        slPrint("STUFF!!!!",slLocate(0,10));
    else //if(gamepadInput | PER_DGT_ST)
        slPrint("NO STUFF!!!!",slLocate(0,10));

    // String s1 = newString();
    // s1->set(s1, "hello");
    int i;
    for (i = 0; i < 4; i++)
    {
        //      i_interaction(g_object_array[i], g_object_array[0]);
        i_draw(g_object_array[i]);

        // slPrint(s1, slLocate(0, 15));
    }

    slSynch();
    return 1;
}
