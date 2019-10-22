#include <sgl.h>
#include "game.h"
#include "Player.h"
#include "Enemy.h"
#include "Item.h"
#include "mem_mgr.h"

Object g_object_array[3];

unsigned int SMMA_MainMode;               /* Game Mode Variable      */
unsigned int SMMA_Mainlevel;              /* Level in the Game Mode  */

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
}

int Game_run()
{
    int i;
    for (i = 0; i < 3; i++)
    {
//      i_interaction(g_object_array[i], g_object_array[0]);
        i_draw(g_object_array[i]);        
    }

    slSynch();
    return 1;
}


