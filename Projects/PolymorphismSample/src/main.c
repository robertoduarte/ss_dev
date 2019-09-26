#include "game.h"

int main(void)
{
    Game_init();
    while (Game_run());
    return 1;
}
