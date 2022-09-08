#pragma once

#include <stddef.h>
#include <stdint.h>
#include "..\Utils\EnhancedLookupBitSet.hpp"

using EntityId = size_t;

static constexpr EntityId EntityCapacity = 512;
static constexpr EntityId InvalidEntityId = EntityCapacity;

struct IdGenerator
{
private:
    static inline EnhancedLookupBitSet<EntityId, EntityCapacity> entities;

public:
    static inline EntityId GenerateId()
    {
        EntityId id = 0;
        if (entities.LookupNextFreePosition(id))
        {
            entities.Set(id);
            return id;
        }
        else
            return InvalidEntityId;
    }

    static inline void FreeId(EntityId id)
    {
        entities.Clear(id);
    }
};