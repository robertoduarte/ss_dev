#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <limits.h>
#include <string.h>

#include "CPUTools.hpp"
#include <yaul.h>
// #include <dbgio/dbgio.h>
#define Debug(format, ...) // dbgio_printf(format, ##__VA_ARGS__)

class SatAlloc
{
private:
    static constexpr bool lockDefault = true;

    static constexpr int32_t abs(int32_t number)
    {
        return (number < 0) ? -number : number;
    }

    using Alignment = unsigned char[8];

    struct Link
    {
        int32_t next;
        int32_t previous;
    };

    union Body
    {
        Link free;
        Alignment data;
    };

    struct Block
    {
        Link header;
        Body body;
    };

    struct HeapData
    {
        size_t address;
        size_t size;
    };

    // static inline Block heap[(256 * 1024) / sizeof(Block)];
    // static inline const size_t HighHeap = (size_t)heap;
    // static inline const size_t HighHeapSize = (sizeof(heap));

    static constexpr size_t LowHeap = 0x00200000;
    static constexpr size_t LowHeapSize = 0x100000;

    static constexpr size_t CartHeap = 0x02400000;
    static constexpr size_t CartHeapSize = 0x400000;

    // static inline const HeapData heapList[] = {{HighHeap, HighHeapSize}};
    static inline const HeapData heapList[] = {{LowHeap, LowHeapSize}};
    // static inline const HeapData heapList[] = {{LowHeap, LowHeapSize},{CartHeap, CartHeapSize}};

    static constexpr size_t heapCount = sizeof(heapList) / sizeof(HeapData);

    static inline int32_t DetectHeap(void *address)
    {
        for (size_t i = 0; i < heapCount; i++)
        {
            if (address >= (char *)heapList[i].address &&
                address < ((char *)heapList[i].address + heapList[i].size))
                return i;
        }

        return -1;
    }

    static inline int32_t GetBlockPosition(void *address, Block list[])
    {
        return ((char *)address - (char *)list) / sizeof(Block);
    }

    static constexpr int32_t MaskAsFree(const int32_t &value)
    {
        return -abs(value);
    }

    static constexpr int32_t ClearMask(const int32_t &value)
    {
        return abs(value);
    }

    static constexpr bool CheckIfFree(const int32_t &value)
    {
        return value < 0;
    }

    static constexpr int32_t SizeToBlocks(size_t size)
    {
        if (size <= sizeof(Alignment))
            return (1);
        size -= (1 + sizeof(Alignment));
        return (2 + size / sizeof(Block));
    }

    static inline void MakeNewBlock(Block list[], const int32_t &c, const int32_t &blocks, const bool &applyMask)
    {
        int32_t nextBlock = ClearMask(list[c].header.next);
        int32_t endBlock = c + blocks;

        list[endBlock].header.next = nextBlock;
        list[endBlock].header.previous = c;
        list[nextBlock].header.previous = endBlock;
        list[c].header.next = applyMask ? MaskAsFree(endBlock) : endBlock;
    }

    static inline void DisconnectFromFreeList(Block list[], const int32_t &c)
    {
        list[list[c].body.free.previous].body.free.next = list[c].body.free.next;
        list[list[c].body.free.next].body.free.previous = list[c].body.free.previous;
        list[c].header.next = ClearMask(list[c].header.next);
    }

    static inline void AssimilateUp(Block list[], const int32_t &c)
    {
        if (CheckIfFree(list[list[c].header.next].header.next))
        {
            Debug("Assimilate up to next free block\n");
            DisconnectFromFreeList(list, list[c].header.next);
            list[ClearMask(list[list[c].header.next].header.next)].header.previous = c;
            list[c].header.next = ClearMask(list[list[c].header.next].header.next);
        }
    }

    static inline int32_t AssimilateDown(Block list[], const int32_t &c, bool applyMask)
    {
        Debug("Assimilate down to next free block\n");
        list[list[c].header.previous].header.next = applyMask ? MaskAsFree(list[c].header.next) : ClearMask(list[c].header.next);
        list[list[c].header.next].header.previous = list[c].header.previous;
        return (list[c].header.previous);
    }

    static inline void FreeFromHeap(Block list[], void *ptr)
    {
        int32_t pos = ((char *)ptr - (char *)(&(list[0]))) / sizeof(Block);
        Debug("Freeing block %d\n", pos);
        AssimilateUp(list, pos);
        if (CheckIfFree(list[list[pos].header.previous].header.next))
            pos = AssimilateDown(list, pos, true);
        else
        {
            Debug("Add to free list head\n");
            list[list[0].body.free.next].body.free.previous = pos;
            list[pos].body.free.next = list[0].body.free.next;
            list[pos].body.free.previous = 0;
            list[0].body.free.next = pos;
            list[pos].header.next = MaskAsFree(list[pos].header.next);
        }
    }

    static inline void *AllocFromHeap(Block list[], const int32_t &listSize, const size_t &size, bool bestFit)
    {
        int32_t blocks = SizeToBlocks(size);
        int32_t cf = list[0].body.free.next;
        int32_t bestBlock = list[0].body.free.next;
        int32_t bestSize = INT_MAX;
        int32_t blockSize = 0;

        while (list[cf].body.free.next)
        {
            blockSize = ClearMask(list[cf].header.next) - cf;
            Debug("Looking at block %d size %d\n", cf, blockSize);
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
                Debug("Alloc %d blocks starting at %d - exact\n", blocks, cf);
                DisconnectFromFreeList(list, cf);
            }

            else
            {
                Debug("Alloc %d blocks starting at %d - existing\n", blocks, cf);
                MakeNewBlock(list, cf, blockSize - blocks, true);
                cf += blockSize - blocks;
            }
        }

        else
        {
            if ((listSize / (int32_t)sizeof(Block)) <= cf + blocks + 1)
            {
                Debug("Can't allocate %d blocks at %d\n", blocks, cf);
                return ((void *)nullptr);
            }

            if (0 == cf)
            {
                Debug("Initializing malloc free block pointer\n");
                list[0].header.next = 1;
                list[0].body.free.next = 1;
                cf = 1;
            }

            Debug("Alloc %d blocks starting at %d - new \n", blocks, cf);
            list[list[cf].body.free.previous].body.free.next = cf + blocks;
            memcpy(&list[cf + blocks], &list[cf], sizeof(Block));
            list[cf].header.next = cf + blocks;
            list[cf + blocks].header.previous = cf;
        }

        return ((void *)&list[cf].body.data);
    }

    static inline Mutex mtx;

public:
    template <bool lock = lockDefault>
    static inline void Free(void *ptr)
    {
        if constexpr (lock)
        {
            mtx.Lock();
            cpu_cache_purge();
        }

        if (!ptr)
        {
            Debug("Free: nullptr, do nothing\n");
            if constexpr (lock)
                mtx.Unlock();
            return;
        }

        int32_t heapIndex = DetectHeap(ptr);
        Debug("Free: %X detected heap %d\n", ptr, heapIndex);

        if (heapIndex >= 0)
            FreeFromHeap((Block *)heapList[heapIndex].address, ptr);

        if constexpr (lock)
            mtx.Unlock();
    }

    template <bool lock = lockDefault>
    static inline void *Malloc(const size_t &size)
    {
        if (size == 0)
        {
            Debug("Malloc: Size 0, do nothing\n");
            return nullptr;
        }

        if constexpr (lock)
        {
            mtx.Lock();
            cpu_cache_purge();
        }

        void *newAddress = nullptr;
        for (size_t i = 0; (newAddress == nullptr) && i < heapCount; i++)
        {
            newAddress = AllocFromHeap((Block *)heapList[i].address, heapList[i].size, size, true);
        }

        if constexpr (lock)
            mtx.Unlock();

        if (!newAddress)
            dbgio_printf("Ram full!");

        return newAddress;
    }

    template <bool lock = lockDefault>
    static inline void *Calloc(const size_t &size)
    {
        if constexpr (lock)
        {
            mtx.Lock();
            cpu_cache_purge();
        }
        void *newAddress = Malloc<false>(size);
        if (newAddress)
            memset(newAddress, 0, size);

        if constexpr (lock)
            mtx.Unlock();

        return newAddress;
    }

    template <bool lock = lockDefault>
    static inline void MemMove(void *dest, void *src, const size_t &size)
    {
        if constexpr (lock)
        {
            mtx.Lock();
            cpu_cache_purge();
        }

        void *buffer = Malloc<false>(size);
        if (buffer)
        {
            memcpy(buffer, src, size);
            memcpy(dest, buffer, size);
            Free<false>(buffer);
        }

        if constexpr (lock)
            mtx.Unlock();
    }

    template <bool lock = lockDefault>
    static inline void *Realloc(void *ptr, const size_t &size)
    {
        if (ptr == nullptr)
        {
            Debug("Realloc: nullptr, use malloc instead\n");
            return Malloc(size);
        }

        if constexpr (lock)
        {
            mtx.Lock();
            cpu_cache_purge();
        }

        int32_t heapIndex = DetectHeap(ptr);
        if (heapIndex < 0)
        {
            if constexpr (lock)
                mtx.Unlock();
            return nullptr;
        }

        Block *list = (Block *)heapList[heapIndex].address;
        int32_t listSize = heapList[heapIndex].size;

        if (0 == size)
        {
            Debug("Realloc: Size 0, use free instead\n");
            Free<false>(ptr);
            if constexpr (lock)
                mtx.Unlock();
            return nullptr;
        }

        void *original = ptr;
        int32_t blocks = SizeToBlocks(size);
        int32_t blockPosition = GetBlockPosition(ptr, &list[0]);
        int32_t blockSize = list[blockPosition].header.next - blockPosition;
        size_t curSize = (blockSize * sizeof(Block)) - sizeof(Link);
        if (blockSize == blocks)
        {
            Debug("realloc the same size block - %d, do nothing\n", blocks);
            if constexpr (lock)
                mtx.Unlock();
            return (ptr);
        }

        AssimilateUp(list, blockPosition);
        if (CheckIfFree(list[list[blockPosition].header.previous].header.next) && (blocks <= (list[blockPosition].header.next - list[blockPosition].header.previous)))
        {
            Debug("realloc() could assimilate down %d blocks - fits!\r\n", blockPosition - list[blockPosition].header.previous);
            DisconnectFromFreeList(list, list[blockPosition].header.previous);
            blockPosition = AssimilateDown(list, blockPosition, false);
            MemMove<false>((void *)&list[blockPosition].body.data, ptr, curSize);
            ptr = (void *)&list[blockPosition].body.data;
        }

        if (blockSize == blocks)
            Debug("realloc the same size block - %d, do nothing\n", blocks);

        else if (blockSize > blocks)
        {
            Debug("realloc %d to a smaller block %d, shrink and free the leftover bits\n", blockSize, blocks);
            MakeNewBlock(list, blockPosition, blocks, false);
            Free<false>((void *)&list[blockPosition + blocks].body.data);
        }

        else
        {
            void *oldptr = ptr;
            Debug("realloc %d to a bigger block %d, make new, copy, and free the old\n", blockSize, blocks);
            if ((ptr = AllocFromHeap(list, listSize, size, 1)))
            {
                memcpy(ptr, oldptr, curSize);
                Free<false>(oldptr);
            }
        }

        if (ptr == nullptr)
        {
            // before giving up, lets try again and hope to find another heap
            void *newAddress = Malloc<false>(size);
            if (newAddress != nullptr)
            {
                Debug("Heap Change %X %X\n", original, newAddress);
                dbgio_printf("Heap Change %X %X\n", original, newAddress);
                memcpy(newAddress, original, curSize);
                Free<false>(original);
                ptr = newAddress;
            }
        }

        if constexpr (lock)
            mtx.Unlock();
        return (ptr);
    }

    template <typename T>
    static inline T *Malloc(const size_t &itemCount = 1)
    {
        return (T *)Malloc(sizeof(T) * itemCount);
    }

    template <typename T>
    static inline T *Calloc(const size_t &itemCount = 1)
    {
        return (T *)Calloc(sizeof(T) * itemCount);
    }

    template <typename T>
    static inline void Realloc(T **ptr, const size_t &itemCount)
    {
        *ptr = (T *)Realloc((void *)*ptr, sizeof(T) * itemCount);
    }

    template <bool lock = lockDefault>
    static inline char *StrDup(const char *src)
    {
        if constexpr (lock)
        {
            mtx.Lock();
            cpu_cache_purge();
        }

        char *dst = (char *)Malloc<false>(strlen(src) + 1);
        if (dst == nullptr)
            return nullptr;
        strcpy(dst, src);

        if constexpr (lock)
            mtx.Unlock();
        return dst;
    }
};

inline void *operator new(size_t size)
{
    // dbgio_printf("new!");
    return SatAlloc::Malloc(size);
}

inline void operator delete(void *p)
{
    // dbgio_printf("delete!");
    SatAlloc::Free(p);
}

inline void operator delete(void *p, unsigned int size)
{
    // dbgio_printf("delete!");
    SatAlloc::Free(p);
}