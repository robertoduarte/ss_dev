#include <SGL.H>
#include "EventSystem/EventManager.h"
#include "ECS\ComponentManager.h"
#include "ECS\Components.h"
#include "ECS\Entity.h"
#include "Frustum\fxMath.h"

static POINT point_SQUARE[] = {
    POStoFIXED(0, 0, 0)};

static POLYGON polygon_SQUARE[] = {
    NORMAL(0.0, 0.0, -1.0),
    VERTICES(0,0, 0, )};

static ATTR attribute_SQUARE[] = {
    ATTRIBUTE(Single_Plane, SORT_MIN, No_Texture, C_RGB(8, 28, 0), No_Gouraud, MESHoff, sprPolygon, UseLight)};

PDATA PD_SQUARE = {point_SQUARE, 4, polygon_SQUARE, 1, attribute_SQUARE};

#define MAX_ENTITIES 500

void init()
{
    slInitSystem(TV_320x240, NULL, 1);
    slPerspective(DEGtoANG(60.0));
    slSetScreenDist(toFIXED(1));
    Entity_Init(MAX_ENTITIES);
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

        ComponentManager_Foreach(positionManager,
        COMPONENT_LAMBDA(Position,
                        {
                            component->x = fxAdd(component->x, toFIXED(100));
                            if (component->x > toFIXED(160))
                            {
                                component->y = fxAdd(component->y, toFIXED(1));
                                component->x = fxSub(component->x,toFIXED(320));
                            }

                            if (component->y > toFIXED(120))
                                component->y = fxSub(component->y, toFIXED(240));
                        }));
        ComponentManager_Foreach(positionManager,
        COMPONENT_LAMBDA(Position,
                        {
                            slPushMatrix();
                            {
                                slTranslate(component->x, component->y, component->z);
                                slPutPolygon(&PD_SQUARE);
                            }
                            slPopMatrix();
                        }));
        slSynch();
    }

    return 1;
}
