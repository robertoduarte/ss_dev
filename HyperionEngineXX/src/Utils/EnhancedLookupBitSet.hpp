#pragma once
#include <stddef.h>
#include <limits.h>
#include "yaul.h"
#include "Timer.hpp"

template <typename IndexType, size_t Capacity>
class EnhancedLookupBitSet
{
private:
    static constexpr IndexType typeCapacity = ~((IndexType)0);
    static_assert(Capacity < (size_t)typeCapacity, "Specified type cannot handle specified capacity");

    static constexpr size_t wordSize = CHAR_BIT * sizeof(size_t);
    static constexpr size_t fullWord = ~((size_t)0);
    static constexpr size_t bitArraySize = Capacity / wordSize;
    static constexpr size_t lookupCacheSize = bitArraySize / wordSize;

    size_t lookupCache[lookupCacheSize] = {};
    size_t bitArray[bitArraySize] = {};

    size_t GetIndex(const IndexType &pos) const
    {
        return ((size_t)pos) / wordSize;
    }

    size_t GetBitMask(const IndexType &pos) const
    {
        return 1 << (((size_t)pos) % wordSize);
    }

    void LookupCacheSet(const IndexType &lookupIndex)
    {
        if (bitArray[lookupIndex] == fullWord)
            lookupCache[GetIndex(lookupIndex)] |= GetBitMask(lookupIndex);
    }

    void LookupCacheClear(const IndexType &lookupIndex)
    {
        lookupCache[GetIndex(lookupIndex)] &= ~GetBitMask(lookupIndex);
    }

    bool CacheCheck(const IndexType &lookupIndex) const
    {
        return lookupCache[GetIndex(lookupIndex)] & GetBitMask(lookupIndex);
    }

public:
    void Set(const IndexType &pos)
    {
        size_t index(GetIndex(pos));
        bitArray[index] |= GetBitMask(pos);
        if constexpr (lookupCacheSize)
            LookupCacheSet(index);
    }

    void Clear(const IndexType &pos)
    {
        size_t index(GetIndex(pos));
        bitArray[index] &= ~GetBitMask(pos);
        if constexpr (lookupCacheSize)
            LookupCacheClear(index);
    }

    bool Check(const IndexType &pos) const
    {
        return bitArray[GetIndex(pos)] & GetBitMask(pos);
    }

    bool LookupNextFreePosition(IndexType &pos)
    {
        size_t skippedWords = 0;
        if constexpr (lookupCacheSize)
        {
            size_t skippedCachePositions = 0;
            while (lookupCache[skippedCachePositions] == fullWord &&
                   skippedCachePositions < lookupCacheSize)
                skippedCachePositions++;

            skippedWords = skippedCachePositions * wordSize;
            while (CacheCheck(skippedWords) && skippedWords < bitArraySize)
                skippedWords++;
        }
        else
            while (skippedWords < bitArraySize && bitArray[skippedWords] == fullWord)
                skippedWords++;

        size_t freePosition = skippedWords * wordSize;
        while (freePosition < Capacity && Check(freePosition))
            freePosition++;
        if (freePosition < Capacity)
        {
            pos = (IndexType)freePosition;
            return true;
        }
        else
            return false;
    }

    bool LookupNextUsedPosition(IndexType &pos)
    {
        size_t skippedWords(GetIndex(pos + 1));
        while (skippedWords < bitArraySize && bitArray[skippedWords] == 0)
            skippedWords++;

        size_t usedPosition = skippedWords * wordSize;
        while (usedPosition < Capacity && !Check(usedPosition))
            usedPosition++;

        if (usedPosition != pos && usedPosition < Capacity)
        {
            pos = (IndexType)usedPosition;
            return true;
        }
        else
            return false;
    }
};
