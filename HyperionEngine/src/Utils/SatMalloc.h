#pragma once

#include <string.h>
#include <stdlib.h>

void *SatMalloc(size_t size);

void *SatCalloc(size_t size);

void SatFree(void *ptr);

void *SatRealloc(void *ptr, size_t size);

void SatMemMove(void *dest, void *src, size_t size);

char *SatStrDup(const char *src);

#define malloc SatMalloc
#define calloc SatCalloc
#define free SatFree
#define realloc SatRealloc
#define memmove SatMemMove
#define strdup SatStrDup
