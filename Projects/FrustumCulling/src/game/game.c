#include "game.h"
#include "Cube.h"

void Game_init(Game* game) {
    slInitSystem(TV_320x240, NULL, 1);
    slPerspective(DEGtoANG(60.0));
    Frustum_init(&game->mainFrustum, 60, R_320x240, F16(1), F16(20000));
}

int Game_run(Game* game) {
    AABox box = {
        {0, 0, F16(1000)},
        F16(25)
    };

    slRotX((65536/1024));
   
    Frustum_updateCam(&game->mainFrustum);

    FRUSTUM_INTERSECTION result = Frustum_boxInFrustum(&game->mainFrustum, &box);

    switch (result) {
        case INSIDE_FRUSTUM:
        {
            slPrint("Inside!  ", slLocate(0, 0));
            slPushMatrix();
            {
                slTranslate(box.center[X], box.center[Y], box.center[Z]);
                slPutPolygon(&PD_CUBE);
            }
            slPopMatrix();
            break;
        }
        case OUTSIDE_FRUSTUM:
        {
            slPrint("Outside! ", slLocate(0, 0));
            break;
        }
    }


    slSynch();
    return 1;
}
