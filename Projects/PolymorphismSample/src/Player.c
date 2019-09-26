#include <sgl.h>
#include "mem_mgr.h"
#include "Player.h"

static ObjectHeader player_header = {
    PLAYER_TYPE_ID,
    (draw_interface) & player_draw,
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
}
