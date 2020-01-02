#include <SGL.H>
#include <SEGA_TIM.H>
#include "EventSystem/EventManager.h"
#include "ECS\ComponentManager.h"
#include "ECS\Components.h"
#include "ECS\Entity.h"

static POINT point_SQUARE[] = {
    POStoFIXED(0, 0, 0)};

static POLYGON polygon_SQUARE[] = {
    NORMAL(0.0, 0.0, -1.0),
    VERTICES(0,0, 0, )};

static ATTR attribute_SQUARE[] = {
    ATTRIBUTE(Single_Plane, SORT_MIN, No_Texture, C_RGB(8, 28, 0), No_Gouraud, MESHoff, sprPolygon, UseLight)};

PDATA PD_SQUARE = {point_SQUARE, 4, polygon_SQUARE, 1, attribute_SQUARE};

#define MAX_ENTITIES 1800

void init()
{
    slInitSystem(TV_320x240, NULL,2);
    slPerspective(DEGtoANG(60.0));
    slDynamicFrame(ON);
    slSetScreenDist(toFIXED(1));
    Entity_Init(MAX_ENTITIES);
    TIM_FRT_INIT(8);
}

int main(void)
{
    init();
    ComponentManager *positionManager = New_ComponentManager(sizeof(Position), MAX_ENTITIES, MAX_ENTITIES);
    for (int i = 0; i < MAX_ENTITIES; i++)
    {
        ComponentManager_CreateComponent(positionManager, Entity_AssignId());
    }

    int x = -160;
    int y = -120;
    
    ComponentManager_Foreach(positionManager,
    COMPONENT_LAMBDA(Position,
        {
            component->x = toFIXED(x);
            component->y = toFIXED(y);
            component->z = toFIXED(1);
            x++;
            if (x > 160)
            {
                y++;
                x -=360;
            }

            if (y > 120)
                y -=240;
        }));

    while (1)
    {

        TIM_FRT_SET_16(0);
        ComponentManager_Foreach(positionManager,
        COMPONENT_LAMBDA(Position,
            {
                component->x += toFIXED(319);
            if (component->x > toFIXED(160))
            {
                component->y += toFIXED(1);
                component->x -= toFIXED(320);
            }
            if (component->y > toFIXED(120))
                component->y -= toFIXED(240);
            }));
        

        ComponentManager_Foreach(positionManager,
        COMPONENT_LAMBDA(Position,
            {
                slPushMatrix();
                {
                    (*point_SQUARE)[X] = component->x;
                    (*point_SQUARE)[Y] = component->y;
                    (*point_SQUARE)[Z] = component->z;
                    slPutPolygon(&PD_SQUARE);
                }
                slPopMatrix();
            }));
        slPrintFX(TIM_FRT_CNT_TO_MCR(TIM_FRT_GET_16()), slLocate(0, 0));
        slSynch();
    }

    return 1;
}
