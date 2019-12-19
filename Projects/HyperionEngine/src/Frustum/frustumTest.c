#include <SL_DEF.H>
#include "frustum.h"
#include "..\Assets\Cube.h"

void frustumTest_run()
{
    slInitSystem(TV_320x240, NULL, 1);
    slPerspective(DEGtoANG(60.0));

    Frustum frustum;
    Frustum_init(&frustum, 60, R_320x240, F16(1), F16(20000));

    AABox box = {
        {0, 0, F16(1000)},
        F16(25)};

    slRotX((65536 / 1024));

    Frustum_updateCam(&frustum);

    FRUSTUM_INTERSECTION result = Frustum_boxInFrustum(&frustum, &box);

    switch (result)
    {
    case INTERSECTS_FRUSTUM:
    case INSIDE_FRUSTUM:
        slPrint("Inside!  ", slLocate(0, 0));
        slPushMatrix();
        {
            slTranslate(box.center[X], box.center[Y], box.center[Z]);
            slPutPolygon(&PD_CUBE);
        }
        slPopMatrix();
        break;
    case OUTSIDE_FRUSTUM:
        slPrint("Outside! ", slLocate(0, 0));
        break;
    }
    slSynch();
}
