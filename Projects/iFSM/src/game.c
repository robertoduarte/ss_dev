#include <sgl.h>
#include "game.h"
#include "Player.h"
#include "Enemy.h"
#include "Item.h"
#include "mem_mgr.h"
#include "sega_per.h"


#define PAD_START  PER_DGT_ST
#define PAD_S   (PAD_START) 
#define PAD_RIGHT  PER_DGT_KR
#define PAD_R   (PAD_RIGHT)
#define PAD_LEFT  PER_DGT_KL
#define PAD_L   (PAD_LEFT)
#define PAD_UP   PER_DGT_KU
#define PAD_U   (PAD_UP)
#define PAD_DOWN  PER_DGT_KD
#define PAD_D   (PAD_DOWN)
#define PAD_A   PER_DGT_TA
#define PAD_B   PER_DGT_TB
#define PAD_C   PER_DGT_TC
#define PAD_X   PER_DGT_TX
#define PAD_Y   PER_DGT_TY
#define PAD_Z   PER_DGT_TZ
#define PAD_TRIG_RIGHT PER_DGT_TR
#define PAD_TR   (PAD_TRIG_RIGHT)
#define PAD_TRIG_LEFT PER_DGT_TL
#define PAD_TL   (PAD_TRIG_LEFT)

Object g_object_array[3];

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

int i = 0;
int Game_run()
{
    int i;
    for (i = 0; i < 3; i++)
    {
//      i_interaction(g_object_array[i], g_object_array[0]);
        i_draw(g_object_array[i]);
    }

    while(1){
        // slPrint((char)dgt_pad_info[10], slLocate(0,5));
    }


    slSynch();
    return 1;
}
