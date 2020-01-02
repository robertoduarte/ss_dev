#include "aabox.h"

inline AABox CreateAABox(FIXED halfExtent, FIXED x, FIXED y, FIXED z)
{
    return (AABox){(FxPoint){x, y, z}, halfExtent};
}

inline FxPoint AABox_getVertexP(AABox *aabox, FxVector normal)
{
    FxPoint result;
    result.x = (normal.x >= 0) ? aabox->center.x + aabox->halfExtent : aabox->center.x - aabox->halfExtent;
    result.y = (normal.y >= 0) ? aabox->center.y + aabox->halfExtent : aabox->center.y - aabox->halfExtent;
    result.z = (normal.z >= 0) ? aabox->center.z + aabox->halfExtent : aabox->center.z - aabox->halfExtent;
    return result;
}

inline FxPoint AABox_getVertexN(AABox *aabox, FxVector normal)
{
    FxPoint result;
    result.x = (normal.x >= 0) ? aabox->center.x - aabox->halfExtent : aabox->center.x + aabox->halfExtent;
    result.y = (normal.y >= 0) ? aabox->center.y - aabox->halfExtent : aabox->center.y + aabox->halfExtent;
    result.z = (normal.z >= 0) ? aabox->center.z - aabox->halfExtent : aabox->center.z + aabox->halfExtent;
    return result;
}
