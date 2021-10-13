#include "SatMalloc.h"
#include <stdbool.h>
#include <limits.h>

#define Debug(format, ...) //Debug(format, ##__VA_ARGS__)

typedef unsigned char Alignment[8];

typedef struct
{
    int next;
    int previous;
} Link;

typedef union
{
    Link free;
    Alignment data;
} Body;

typedef struct
{
    Link header;
    Body body;
} Block;

typedef struct
{
    void *address;
    size_t size;
} HeapData;

// static Block heap[(128 * 1024) / sizeof(Block)];s
// #define HighHeap (void *)heap
// #define HighHeapSize (sizeof(heap))

#define LowHeap (void *)0x00200000
#define LowHeapSize 0x100000
#define CartHeap (void *)0x02400000
#define CartHeapSize 0x400000

const HeapData heapList[] = {{LowHeap, LowHeapSize}, {CartHeap, CartHeapSize}};

const int heapCount = sizeof(heapList) / sizeof(HeapData);

static int DetectHeap(void *address)
{
    for (int i = 0; i < heapCount; i++)
    {
        if (address >= heapList[i].address &&
            address < (heapList[i].address + heapList[i].size))
            return i;
    }
    return -1;
}

static int GetBlockPosition(void *address, Block list[])
{
    return (address - (void *)list) / sizeof(Block);
}

static int MaskAsFree(int value)
{
    return -abs(value);
}

static int ClearMask(int value)
{
    return abs(value);
}

static bool CheckIfFree(int value)
{
    return value < 0;
}

static int SizeToBlocks(size_t size)
{
    if (size <= sizeof(Alignment))
        return (1);
    size -= (1 + sizeof(Alignment));
    return (2 + size / sizeof(Block));
}

static void MakeNewBlock(Block list[], int c, int blocks, bool applyMask)
{
    int nextBlock = ClearMask(list[c].header.next);
    int endBlock = c + blocks;

    list[endBlock].header.next = nextBlock;
    list[endBlock].header.previous = c;
    list[nextBlock].header.previous = endBlock;
    list[c].header.next = applyMask ? MaskAsFree(endBlock) : endBlock;
}

static void DisconnectFromFreeList(Block list[], int c)
{
    list[list[c].body.free.previous].body.free.next = list[c].body.free.next;
    list[list[c].body.free.next].body.free.previous = list[c].body.free.previous;
    list[c].header.next = ClearMask(list[c].header.next);
}

static void AssimilateUp(Block list[], int c)
{
    if (CheckIfFree(list[list[c].header.next].header.next))
    {
        Debug("Assimilate up to next free block");
        DisconnectFromFreeList(list, list[c].header.next);
        list[ClearMask(list[list[c].header.next].header.next)].header.previous = c;
        list[c].header.next = ClearMask(list[list[c].header.next].header.next);
    }
}

static int AssimilateDown(Block list[], int c, bool applyMask)
{
    Debug("Assimilate down to next free block");
    list[list[c].header.previous].header.next = applyMask ? MaskAsFree(list[c].header.next) : ClearMask(list[c].header.next);
    list[list[c].header.next].header.previous = list[c].header.previous;
    return (list[c].header.previous);
}

static void FreeFromHeap(Block list[], void *ptr)
{
    int pos = (ptr - (void *)(&(list[0]))) / sizeof(Block);
    Debug("Freeing block %d", pos);
    AssimilateUp(list, pos);
    if (CheckIfFree(list[list[pos].header.previous].header.next))
        pos = AssimilateDown(list, pos, true);
    else
    {
        Debug("Add to free list head");
        list[list[0].body.free.next].body.free.previous = pos;
        list[pos].body.free.next = list[0].body.free.next;
        list[pos].body.free.previous = 0;
        list[0].body.free.next = pos;
        list[pos].header.next = MaskAsFree(list[pos].header.next);
    }
}

void SatFree(void *ptr)
{
    if (!ptr)
    {
        Debug("SatFree: NULL pointer, do nothing");
        return;
    }
    int heapIndex = DetectHeap(ptr);
    Debug("SatFree: %X detected heap %d", ptr, heapIndex);

    if (heapIndex >= 0)
        FreeFromHeap(heapList[heapIndex].address, ptr);
}

void *AllocFromHeap(Block list[], int listSize, size_t size, bool bestFit)
{
    int blocks = SizeToBlocks(size);
    int cf = list[0].body.free.next;
    int bestBlock = list[0].body.free.next;
    int bestSize = INT_MAX;
    int blockSize = 0;

    while (list[cf].body.free.next)
    {
        blockSize = ClearMask(list[cf].header.next) - cf;
        Debug("Looking at block %d size %d", cf, blockSize);
        if (bestFit)
        {
            if ((blockSize >= blocks) && (blockSize < bestSize))
            {
                bestBlock = cf;
                bestSize = blockSize;
            }
        }
        else
        {
            if ((blockSize >= blocks))
                break;
        }
        cf = list[cf].body.free.next;
    }
    if (INT_MAX != bestSize)
    {
        cf = bestBlock;
        blockSize = bestSize;
    }
    if (ClearMask(list[cf].header.next))
    {
        if (blockSize == blocks)
        {
            Debug("Alloc %d blocks starting at %d - exact", blocks, cf);
            DisconnectFromFreeList(list, cf);
        }
        else
        {
            Debug("Alloc %d blocks starting at %d - existing", blocks, cf);
            MakeNewBlock(list, cf, blockSize - blocks, true);
            cf += blockSize - blocks;
        }
    }
    else
    {
        if ((listSize / sizeof(Block)) <= cf + blocks + 1)
        {
            Debug("Can't allocate %d blocks at %d", blocks, cf);
            return ((void *)NULL);
        }
        if (0 == cf)
        {
            Debug("Initializing malloc free block pointer");
            list[0].header.next = 1;
            list[0].body.free.next = 1;
            cf = 1;
        }
        Debug("Alloc %d blocks starting at %d - new ", blocks, cf);
        list[list[cf].body.free.previous].body.free.next = cf + blocks;
        memcpy(&list[cf + blocks], &list[cf], sizeof(Block));
        list[cf].header.next = cf + blocks;
        list[cf + blocks].header.previous = cf;
    }
    return ((void *)&list[cf].body.data);
}

void *SatMalloc(size_t size)
{
    if (size == 0)
    {
        Debug("SatMalloc: Size 0, do nothing");
        return NULL;
    }
    void *newAddress = NULL;
    for (int i = 0; (newAddress == NULL) && i < heapCount; i++)
    {
        newAddress = AllocFromHeap(heapList[i].address, heapList[i].size, size, true);
    }
    return newAddress;
}

void *SatCalloc(size_t size)
{
    void *newAddress = SatMalloc(size);
    if (newAddress)
        memset(newAddress, 0, size);

    return newAddress;
}

void SatMemMove(void *dest, void *src, size_t size)
{
    void *buffer = SatMalloc(size);
    if (buffer)
    {
        memcpy(buffer, src, size);
        memcpy(dest, buffer, size);
        SatFree(buffer);
    }
}

void *SatRealloc(void *ptr, size_t size)
{

    if (ptr == NULL)
    {
        Debug("SatRealloc: NULL pointer, use malloc instead");
        return SatMalloc(size);
    }

    int heapIndex = DetectHeap(ptr);
    if (heapIndex < 0)
        return NULL;

    Block *list = heapList[heapIndex].address;
    int listSize = heapList[heapIndex].size;

    if (0 == size)
    {
        Debug("SatRealloc: Size 0, use free instead");
        SatFree(ptr);
        return NULL;
    }
    void *original = ptr;
    int blocks = SizeToBlocks(size);
    int blockPosition = GetBlockPosition(ptr, &list[0]);
    int blockSize = list[blockPosition].header.next - blockPosition;
    size_t curSize = (blockSize * sizeof(Block)) - sizeof(Link);
    if (blockSize == blocks)
    {
        Debug("realloc the same size block - %d, do nothing", blocks);
        return (ptr);
    }
    AssimilateUp(list, blockPosition);
    if (CheckIfFree(list[list[blockPosition].header.previous].header.next) && (blocks <= (list[blockPosition].header.next - list[blockPosition].header.previous)))
    {
        Debug("realloc() could assimilate down %d blocks - fits!\r", blockPosition - list[blockPosition].header.previous);
        DisconnectFromFreeList(list, list[blockPosition].header.previous);
        blockPosition = AssimilateDown(list, blockPosition, false);
        SatMemMove((void *)&list[blockPosition].body.data, ptr, curSize);
        ptr = (void *)&list[blockPosition].body.data;
    }
    if (blockSize == blocks)
        Debug("realloc the same size block - %d, do nothing", blocks);

    else if (blockSize > blocks)
    {
        Debug("realloc %d to a smaller block %d, shrink and free the leftover bits", blockSize, blocks);
        MakeNewBlock(list, blockPosition, blocks, false);
        SatFree((void *)&list[blockPosition + blocks].body.data);
    }
    else
    {
        void *oldptr = ptr;
        //Debug("realloc %d to a bigger block %d, make new, copy, and free the old", blockSize, blocks);
        if ((ptr = AllocFromHeap(list, listSize, size, 1)))
        {
            memcpy(ptr, oldptr, curSize);
            SatFree(oldptr);
        }
    }
    if (ptr == NULL)
    {
        // before giving up, lets try again and hope to find another heap
        void *newAddress = SatMalloc(size);
        if (newAddress != NULL)
        {
            Debug("Heap Change %X %X", original, newAddress);
            memcpy(newAddress, original, curSize);
            SatFree(original);
            ptr = newAddress;
        }
    }
    return (ptr);
}

char *SatStrDup(const char *src)
{
    char *dst = SatMalloc(strlen(src) + 1);
    if (dst == NULL)
        return NULL;
    strcpy(dst, src);
    return dst;
}
