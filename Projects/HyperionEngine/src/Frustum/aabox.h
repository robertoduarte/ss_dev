#ifndef AABOX_H
#define AABOX_H

#include <sl_def.h>

typedef struct
{
    POINT center;
    FIXED halfExtent;
} AABox;

void AABox_getVertexP(AABox* aabox, VECTOR normal, FIXED result[XYZ]);

void AABox_getVertexN(AABox* aabox, VECTOR normal, FIXED result[XYZ]);

#endif /* AABOX_H */

