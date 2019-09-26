#include "game/game.h"

int main(void)
{
    Game game;
    Game_init(&game);
    while (Game_run(&game));
    return 1;
}
