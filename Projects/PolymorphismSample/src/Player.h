#ifndef PLAYER_H
#define PLAYER_H

#include "Object.h"

#define PLAYER_TYPE_ID 1

typedef struct
{
    /*------Object Header------*/
    ObjectHeader* header;
    /*-----Object Variables----*/
    FIXED test_val;
} Player;

Player* new_Player();
void player_initialize(Player* player);
void player_draw(Player* player);

#endif /* PLAYER_H */

