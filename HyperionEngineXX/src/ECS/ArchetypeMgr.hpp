#pragma once
#include <stdint.h>

#include "EntityId.hpp"
#include "ArchetypeId.hpp"

#include "stddef.h"
#include "..\Utils\RBTree.hpp"
#include "..\Utils\Vector.hpp"
#include "..\Utils\SatAlloc.hpp"

using MgrIndex = int16_t;
using TypeRow = uint16_t;

template <typename... SupportedTypes>
class ArchetypeMgr
{
public:
    using ArchetypeIdST = ArchetypeId<SupportedTypes...>;
    const ArchetypeIdST type;

private:
    const size_t dataSize;
    size_t capacity;
    size_t size;
    size_t offsets[ArchetypeIdST::NonTagComponentCount()] = {};
    void *entities;

    inline char *EntityAddress(const TypeRow &row) const
    {
        return (char *)entities + (dataSize * row);
    }

    inline char *EntityComponents(const TypeRow &row) const
    {
        return EntityAddress(row) + sizeof(EntityId);
    }

public:
    inline ArchetypeMgr(const ArchetypeIdST &a) : type(a),
                                                  dataSize(sizeof(EntityId) + type.Size()),
                                                  capacity(64),
                                                  size(0),
                                                  entities(SatAlloc::Malloc(dataSize * capacity))
    {
        if (!type.Empty())
            ((!std::is_empty_v<SupportedTypes> ? offsets[ArchetypeIdST::template ComponentIndex<SupportedTypes>()] = type.template GetOffset<SupportedTypes>() : false), ...);
    }

    inline TypeRow Add(const EntityId &entityId)
    {
        void *data = nullptr;
        if (size >= capacity)
        {
            // capacity = (capacity << 1) - (capacity >> 1);
            capacity *= 2;
            entities = SatAlloc::Realloc(entities, dataSize * capacity);
        }

        data = (char *)entities + (dataSize * size);
        size++;

        if (data)
        {
            memset(data, 0, dataSize);
            *(EntityId *)data = entityId;
        }

        return size - 1;
    }

    inline uint16_t Last() const { return size ? size - 1 : 0; }

    inline uint16_t Size() const { return size; }

    inline bool Empty() const { return size == 0; }

    inline EntityId GetId(const TypeRow &row) const
    {
        return *((EntityId *)EntityAddress(row));
    }

    inline EntityId *RemoveRow(const TypeRow &row)
    {
        uint16_t last = Last();
        if (!Empty())
            size--;

        if (row == last)
            return nullptr;

        memcpy(EntityAddress(row), EntityAddress(last), dataSize);
        return (EntityId *)EntityAddress(row);
    }

    template <typename Component>
    Component *Get(const TypeRow &row) const
    {
        if constexpr (std::is_empty_v<Component>)
            return nullptr;
        else
            return (Component *)(EntityComponents(row) + offsets[ArchetypeIdST::template ComponentIndex<Component>()]);
    }

private:
    static inline Vector<ArchetypeMgr *> managers;
    static inline RBTree<ArchetypeIdST, MgrIndex> managerMap;

public:
    static inline MgrIndex Find(const ArchetypeIdST &a)
    {
        if (MgrIndex *index = managerMap.Search(a))
            return *index;

        managers.Push(new ArchetypeMgr(a));
        managerMap.Insert(a, managers.Last());
        return managers.Last();
    }

    static inline ArchetypeMgr *Access(const MgrIndex &index)
    {
        return managers[index];
    }

    static inline size_t Count()
    {
        return managers.Size();
    }
};
