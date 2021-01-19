#ifndef SAT_MALLOC_H
#define SAT_MALLOC_H

#include <stdlib.h>

void *SatMalloc(size_t size);

void *SatCalloc(size_t size);

void SatFree(void *ptr);

void *SatRealloc(void *ptr, size_t size);

void SatMemMove(void *dest, void *src, size_t size);

#define malloc SatMalloc
#define calloc SatCalloc
#define free SatFree
#define realloc SatRealloc
#define memmove SatMemMove

#endif //SAT_MALLOC_H
