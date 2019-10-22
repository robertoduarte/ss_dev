#include "Enemy.h"

static ObjectHeader enemy_header = {
    ENEMY_TYPE_ID,
    (draw_interface) & enemy_draw, //Is it a subscribe?¿
    (interaction_interface) & enemy_interaction
};

Enemy * new_Enemy()
{
    Enemy* enemy = Memmalloc(sizeof (Enemy)); //malloc;
    if (enemy) enemy_initialize(enemy);
    return enemy;
}

void enemy_initialize(Enemy * enemy)
{
    enemy->header = &enemy_header;
    enemy->test_val = 0;
}

void enemy_interaction(Enemy* enemy, Player * player)
{
    if (!is_type(enemy, ENEMY_TYPE_ID) || !is_type(player, PLAYER_TYPE_ID)) return;
    enemy->test_val = enemy->test_val + player->test_val;
}

void enemy_draw(Enemy * enemy)
{
    if (!is_type(enemy, ENEMY_TYPE_ID)) return;
    slPrintFX(enemy->test_val, slLocate(0, 0));
    slPrint("Draw enemy", slLocate(10,4));
}

void enemy_update()
{
    State _state;
    switch(_state){

    }
}

