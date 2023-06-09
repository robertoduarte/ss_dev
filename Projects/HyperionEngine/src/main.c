#include <SL_DEF.H>
#include <SEGA_TIM.H>
#include "Utils\Debug.h"
#include "EventSystem\EventManager.h"
#include "InputSystem\InputSystem.h"
#include "Geometry\Frustum.h"
#include "Assets\Cube.h"
#include "Assets\Model.h"
#include "ECS/EntityManager.h"
#include "ECS/Component.h"

// extern Uint16 TotalPolygons;
// Debug_PrintLine("Poly count: %u", TotalPolygons);
// TIM_FRT_INIT(TIM_CKS_128);
// TIM_FRT_SET_16(0);
// Debug_PrintLine("Ticks elapsed %d", TIM_FRT_GET_16());
// Debug_PrintMsg("File:%s Line:%d", __FILE__, __LINE__);

Fxp3D posistion = {0, 0, 0};
Angle angleX = 0;
Angle angleY = 0;

const Angle increment = DEG_TO_ANGLE(1);

void HandleInput(EventType type, Action action)
{
    switch (action)
    {
    case (Action_TurnRight):
        angleY += increment;
        break;
    case (Action_TurnLeft):
        angleY -= increment;
        break;
    case (Action_TurnDown):
        angleX -= increment;
        break;
    case (Action_TurnUp):
        angleX += increment;
        break;
    default:
        break;
    }
}

int main()
{
    slInitSystem(TV_320x240, NULL, 1);
    slPerspective(DEG_TO_ANGLE(60.0));

    int x = 10;
    while (x--)
        Debug_PrintMsg("Created entity: %d.",
                       EntityManager_CreateEntity(ComponentId_Position | ComponentId_Motion));

    EventManager_AddListener(Action_Event, (EventListener)HandleInput);

    Frustum frustum = CreateFrustum(DEG_TO_ANGLE(62.0), AspectRatio_320x256, FXP_SET(200.0), FXP_SET(6000.0));

    Fxp matrix[4][3];

    Fxp test = FXP_SET(399);

    while (1)
    {
        InputSystem_Update();
        EventManager_Update();
        slPushMatrix();
        {
            slRotY(angleY);
            slRotX(angleX);
            slGetMatrix(matrix);

            Frustum_Update(&frustum, matrix);

            Fxp3D objectPosition = {0, 0, test};
            Fxp objectSize = FXP_SET(50);

            if (Frustum_BoxInFrustum(&frustum, &objectPosition, objectSize))
            {
                slPushMatrix();
                {
                    slTranslate(objectPosition.x, objectPosition.y, objectPosition.z);
                    slPutPolygon(&MeshModel);
                    slPutPolygon(&PD_CUBE);
                }
                slPopMatrix();
                Debug_PrintLine("INSIDE");
            }
            else
            {
                Debug_PrintLine("OUTSIDE");
            }
        }
        slPopMatrix();
        slSynch();
    }
    return 1;
}
