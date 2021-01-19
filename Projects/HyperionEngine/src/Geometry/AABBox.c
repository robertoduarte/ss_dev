#include "AABBox.h"

Fxp3D AABBox_GetVertex(Fxp3DPtr position, Fxp size, Fxp3DPtr normal, VertexType type)
{
    Fxp halfsize = Fxp_Div(size, FXP_SET(2));
    Fxp halfExtent = (type == VertexType_N) ? halfsize : -halfsize;

    Fxp3D result;
    result.x = (normal->x >= 0) ? position->x + halfExtent : position->x - halfExtent;
    result.y = (normal->y >= 0) ? position->y + halfExtent : position->y - halfExtent;
    result.z = (normal->z >= 0) ? position->z + halfExtent : position->z - halfExtent;

    return result;
}
