// #include <SL_DEF.H>
// #include "frustum.h"
// #include "..\Assets\Cube.h"

// void main()
// {
//     slInitSystem(TV_320x240, NULL, 1);
//     slPerspective(DEGtoANG(60.0));

//     Frustum frustum = CreateFrustum(60, R_320x240, toFIXED(10), toFIXED(20000));
//     AABox box = CreateAABox(toFIXED(25), 0, 0, toFIXED(1000));

//     while (1)
//     {
//         slRotX(65536 / 1024);
//         slRotY(65536 / 1024);

//         Frustum_updateCam(&frustum);

//         switch (Frustum_boxInFrustum(&frustum, &box))
//         {
//         case INTERSECTS_FRUSTUM:
//         case INSIDE_FRUSTUM:
//             slPrint("Inside!  ", slLocate(0, 0));
//             slPushMatrix();
//             {
//                 slTranslate(box.center.x, box.center.y, box.center.z);
//                 slPutPolygon(&PD_CUBE);
//             }
//             slPopMatrix();
//             break;
//         case OUTSIDE_FRUSTUM:
//             slPrint("Outside! ", slLocate(0, 0));
//             break;
//         }
//         slSynch();
//     }
// }
