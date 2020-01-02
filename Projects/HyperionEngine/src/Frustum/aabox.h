#ifndef AABOX_H
#define AABOX_H

#include <sl_def.h>
#include "vectorMath.h"

typedef struct
{
    FxPoint center;
    FIXED halfExtent;
} AABox;

inline AABox CreateAABox(FIXED halfExtent, FIXED x, FIXED y, FIXED z);

inline FxPoint AABox_getVertexP(AABox *aabox, FxVector normal);

inline FxPoint AABox_getVertexN(AABox *aabox, FxVector normal);

#endif /* AABOX_H */

