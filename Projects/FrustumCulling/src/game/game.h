#ifndef EPIMETHEUS_H
#define EPIMETHEUS_H

#include "frustum/frustum.h"

typedef struct
{
    Frustum mainFrustum;
} Game;

void Game_init(Game* epimetheus);

int Game_run(Game* epimetheus);

#endif /* EPIMETHEUS_H */

