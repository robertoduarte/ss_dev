#include <sgl.h>
#include "Utils/mem_mgr.h"
#include "Player.h"
#include "Utils/debuglog.h"


static ObjectHeader player_header = {
    PLAYER_TYPE_ID,
    (draw_interface) & player_draw, //Subscribe?
    UNUSED_INTERFACE
};

Player* new_Player()
{
    Player* player = Memmalloc(sizeof (Player));
    if (player) player_initialize(player);
    return player;
}

void player_initialize(Player* player)
{
    player->header = &player_header;
    player->test_val = toFIXED(1);
}

void player_draw(Player* player)
{
    if (!is_type(player, PLAYER_TYPE_ID)) return;
    slPrintFX(player->test_val, slLocate(0, 1));
    slPrint("Draw player", slLocate(10,3));
}
