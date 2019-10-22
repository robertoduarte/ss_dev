#ifndef ENEMY_H
#define ENEMY_H

#include <sgl.h>
#include "mem_mgr.h"
#include "Player.h"
#include "Object.h"


#define ENEMY_TYPE_ID 2

typedef struct
{
    /*------Object Header------*/
    ObjectHeader* header;
    /*-----Object Variables----*/
    FIXED test_val;
} Enemy;

Enemy * new_Enemy();
void enemy_initialize(Enemy* enemy);

void enemy_draw(Enemy* enemy);
void enemy_interaction(Enemy* enemy, Player* player_ptr);

#endif /* ENEMY_H */

