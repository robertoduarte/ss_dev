#include <sgl.h>
#include "game.h"
#include "Player.h"
// #include "Enemy.h"
// #include "Item.h"
#include "mem_mgr.h"
#include "Controller/control.h"

Object g_object_array[4];

void Game_init()
{
    slInitSystem(TV_320x240, NULL, 1);
    slPerspective(DEGtoANG(60.0));

    // use 512K of work RAM for general heap
    // handle set size = 16, trash RAM = 2K
    MemInit((pointer) 0x06040000, (pointer) 0x060BFFFF, 16, 16, 0x800);

    g_object_array[0] = new_Player();
    g_object_array[1] = new_Enemy();
    g_object_array[2] = new_Item();
    g_object_array[3] = new_Input();
}

int GetInput(Input i)
{
    // if(i.gamepadInput == 1){
        // slPrint(i.gamepadInput,slLocate(0,15));
    // }
    return i.gamepadInput;
}

int Game_run()
{
    // int gamepadInput = Smpc_Peripheral[0].data;

    // if(!(gamepadInput & PER_DGT_ST))
    //     slPrint("STUFF!!!!",slLocate(0,10));
    // else if(gamepadInput | PER_DGT_ST)
    //     slPrint("NO STUFF!!!!",slLocate(0,10));

    
        

    int i;
    for (i = 0; i < 4; i++)
    {
//      i_interaction(g_object_array[i], g_object_array[0]);
        i_draw(g_object_array[i]);

        slPrint(GetInput(*new_Input()), slLocate(0,15));
    }

    slSynch();
    return 1;
}


