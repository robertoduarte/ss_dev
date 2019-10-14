#include <sgl.h>
#include "game.h"
#include "Utils/mem_mgr.h"


void Game_init()
{
    slInitSystem(TV_320x240, NULL, 1);
    slPerspective(DEGtoANG(60.0));

    // use 512K of work RAM for general heap
    // handle set size = 16, trash RAM = 2K
    MemInit((pointer) 0x06040000, (pointer) 0x060BFFFF, 16, 16, 0x800);

}

int Game_run()
{
    slSynch();
    return 1;
}
