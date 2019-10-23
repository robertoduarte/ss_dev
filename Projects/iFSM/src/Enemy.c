#include "Enemy.h"

typedef enum{
    STATE_STANDING,
    STATE_PATROL,
    STATE_SLEEP,
    STATE_AGRESSIVE,
    STATE_DEAD
}EnemyFSM;


static ObjectHeader enemy_header = {
    ENEMY_TYPE_ID,
    (draw_interface) & enemy_draw, 
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

int i = 0;
void enemy_update()
{
    EnemyFSM _state;
    
    switch (_state)
    {
    case STATE_PATROL:
        /* code */
        break;
    case STATE_SLEEP:
        /* code */
        break;
    case STATE_STANDING:
        /* code S */
        break;
    case STATE_AGRESSIVE:
        /* code */
        break;
    case STATE_DEAD:
        /* code */
        break;
            
    default: 
        break;
    }
}

