#include "IdGenerator.h"
#include "SatMalloc.h"

#include <stdlib.h>
#include <limits.h>

#define BLOCK_TYPE unsigned int
#define WORD_SIZE CHAR_BIT * sizeof(int)

#define FULL_BLOCK ((BLOCK_TYPE) ~(BLOCK_TYPE)0)
#define TOTAL_BLOCKS ((ID_CAPACITY / WORD_SIZE) + 1)

#define SET_BIT(A, k) (A[(k / WORD_SIZE)] |= (1 << (k % WORD_SIZE)))
#define CLEAR_BIT(A, k) (A[(k / WORD_SIZE)] &= ~(1 << (k % WORD_SIZE)))
#define TEST_BIT(A, k) (A[(k / WORD_SIZE)] & (1 << (k % WORD_SIZE)))

struct IdGenerator
{
    int capacity;
    int totalBlocks;
    BLOCK_TYPE *idBits;
};

IdGenerator *
IdGenerator_Init(int capacity)
{
    if (!capacity)
        return NULL;
    IdGenerator *idGenerator = malloc(sizeof(IdGenerator));
    idGenerator->capacity = abs(capacity);
    idGenerator->totalBlocks = ((idGenerator->capacity / WORD_SIZE) + 1);
    idGenerator->idBits = malloc(sizeof(BLOCK_TYPE) * idGenerator->totalBlocks);
    return idGenerator;
}

void IdGenerator_FreeId(IdGenerator *idGenerator, int id)
{
    if (!idGenerator)
        return;
    if (id >= 0 && id < idGenerator->capacity)
        CLEAR_BIT(idGenerator->idBits, id);
}

int IdGenerator_GenerateId(IdGenerator *idGenerator)
{
    if (!idGenerator)
        return -1;

    int skippedBlocks = 0;
    while (idGenerator->idBits[skippedBlocks] == FULL_BLOCK && skippedBlocks < idGenerator->totalBlocks)
    {
        skippedBlocks++;
    }
    int nextIdBit = skippedBlocks * WORD_SIZE;
    while (nextIdBit < idGenerator->capacity && TEST_BIT(idGenerator->idBits, nextIdBit))
    {
        nextIdBit++;
    }
    if (nextIdBit >= idGenerator->capacity)
    {
        return -1;
    }
    else
    {
        SET_BIT(idGenerator->idBits, nextIdBit);
        return nextIdBit;
    }
}
