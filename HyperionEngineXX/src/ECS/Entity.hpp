#pragma once

#include "ArchetypeMgr.hpp"
#include "..\Utils\StringUtils.hpp"
#include "yaul.h"

template <typename... SupportedTypes>
class Entity
{
    // Static Stuff
private:
    static Entity invalidEntity;
    static Entity entities[EntityCapacity];

public:
    using ArchetypeIdST = ArchetypeId<SupportedTypes...>;
    using ArchetypeMgrST = ArchetypeMgr<SupportedTypes...>;

    static inline Entity &Create(const ArchetypeIdST &archetype = ArchetypeIdST())
    {
        EntityId id = IdGenerator::GenerateId();

        if (id == InvalidEntityId)
            return invalidEntity;

        Entity &entity = entities[id];

        entity.managerIndex = ArchetypeMgrST::Find(archetype);
        entity.row = ArchetypeMgrST::Access(entity.managerIndex)->Add(id);

        return entity;
    }

    template <typename... T>
    static Entity &Create()
    {
        return Create(ArchetypeIdST::template ConstArchetypeId<T...>());
    }

    template <typename... T>
    static Entity &Create(const T &...component)
    {
        Entity &entity = Create<T...>();
        if (entity.IsValid())
            ((!std::is_empty_v<T> ? ((*entity.Get<T, false>() = component), true) : true), ...);

        return entity;
    }

    static inline Entity &GetEntity(const EntityId &id)
    {
        if (id < EntityCapacity)
            return entities[id];
        else
            return invalidEntity;
    }

    static inline void Delete(const EntityId &id)
    {
        if (id < EntityCapacity)
        {
            entities[id].Remove();
            IdGenerator::FreeId(id);
        }
    }

    // Member Stuff
private:
    MgrIndex managerIndex;
    TypeRow row;

    template <typename T>
    void CopyComponent(const Entity &srcEntity)
    {
        if constexpr (!std::is_empty_v<T>)
        {
            if (GetType().template Contains<T>() && srcEntity.GetType().template Contains<T>())
                *(Get<T>()) = *(srcEntity.Get<T>());
        }
    }

    inline void CopyComponents(const Entity &srcEntity)
    {
        (CopyComponent<SupportedTypes>(srcEntity), ...);
    }

    inline void Replace(const Entity &srcEntity)
    {
        Remove();
        managerIndex = srcEntity.managerIndex;
        row = srcEntity.row;
    }

    Entity() : managerIndex(-1), row(0){};

    Entity(const ArchetypeIdST &a, const Entity &srcEntity)
    {
        managerIndex = ArchetypeMgrST::Find(a);
        row = ArchetypeMgrST::Access(managerIndex)->Add(srcEntity.GetId());
        CopyComponents(srcEntity);
    }

    template <typename T>
    void SetComponent(const char *data)
    {
        if constexpr (!std::is_empty_v<T>)
        {
            if (*data != '\0')
            {
                T temp;
                StringUtils::Parse(temp, data);
                AddComponent(temp);
                return;
            }
        }
        AddComponent<T>();
    }

    inline void Remove()
    {
        if (IsValid())
        {
            ArchetypeMgrST *manager = ArchetypeMgrST::Access(managerIndex);
            if (EntityId *id = manager->RemoveRow(row))
                entities[*id].row = row;
        }
    }

public:
    inline const ArchetypeIdST &GetType() const
    {
        return ArchetypeMgrST::Access(managerIndex)->type;
    }

    inline EntityId GetId() const
    {
        return ArchetypeMgrST::Access(managerIndex)->GetId(row);
    }

    inline bool IsValid() const
    {
        return managerIndex != -1;
    }

    template <typename T, bool check = false>
    T *Get() const
    {
        if constexpr (check)
            return IsValid() ? ArchetypeMgrST::Access(managerIndex)->template Get<T>(row) : nullptr;
        else
            return ArchetypeMgrST::Access(managerIndex)->template Get<T>(row);
    }

    template <typename T>
    Entity &AddComponent()
    {
        if (IsValid())
        {
            constexpr ArchetypeIdST component = ArchetypeIdST::template ConstArchetypeId<T>();
            ArchetypeIdST type = GetType();
            if (!type.template Contains(component))
                Replace(Entity(type + component, *this));
        }
        return *this;
    }

    template <typename... T>
    Entity &AddComponent(const T &...component)
    {
        if (IsValid())
        {
            constexpr ArchetypeIdST components = ArchetypeIdST::template ConstArchetypeId<T...>();
            ArchetypeIdST type = GetType();
            if (!type.template Contains(components))
                Replace(Entity(type + components, *this));

            ((*Get<T>() = component), ...);
        }
        return *this;
    }

    inline Entity &AddComponent(const char *name, const char *data)
    {
        ((MatchesTypeName<SupportedTypes>(name) ? (SetComponent<SupportedTypes>(data), false) : true) && ...);
        return *this;
    }

    template <typename... T>
    Entity &RemoveComponents()
    {
        if (IsValid())
        {
            constexpr ArchetypeIdST components = ArchetypeIdST::template ConstArchetypeId<T...>();
            ArchetypeIdST type = GetType();
            if (type.template Contains(components))
                Replace(Entity(type - components, *this));
        }
        return *this;
    }

private:
    template <class T>
    struct ForEachHelper;

    template <class Ret, class T, typename... Components>
    struct ForEachHelper<Ret (T::*)(EntityId, Components &...) const>
    {
        static_assert((!std::is_empty_v<Components> && ...), "Cannot access Tag types in ForEach Lambda.");

        constexpr static ArchetypeIdST type = ArchetypeIdST::template ConstArchetypeId<Components...>();

        template <typename Lambda>
        static void Execute(ArchetypeMgrST *manager, const TypeRow &row, Lambda lambda)
        {
            lambda(manager->GetId(row), *manager->template Get<Components>(row)...);
        }
    };

    template <class Ret, class T, typename... Components>
    struct ForEachHelper<Ret (T::*)(Components &...) const>
    {
        static_assert((!std::is_empty_v<Components> && ...), "Cannot access Tag types in ForEach Lambda.");

        constexpr static ArchetypeIdST type = ArchetypeIdST::template ConstArchetypeId<Components...>();

        template <typename Lambda>
        static void Execute(ArchetypeMgrST *manager, const TypeRow &row, Lambda lambda)
        {
            lambda(*manager->template Get<Components>(row)...);
        }
    };

public:
    template <typename... Components, typename Lambda>
    static void ForEach(Lambda lambda)
    {
        auto helper = ForEachHelper<decltype(&Lambda::operator())>();

        constexpr static ArchetypeIdST signatureType = ArchetypeIdST::template ConstArchetypeId<Components...>();

        if constexpr (!signatureType.Empty())
            static_assert(signatureType.template Contains(helper.type), "Lambda component types not present in ForEach signature types.");

        constexpr static ArchetypeIdST type = signatureType + helper.type;

        for (size_t i = 0; i < ArchetypeMgrST::Count(); i++)
        {
            ArchetypeMgrST *manager = ArchetypeMgrST::Access(i);
            if (manager->type.template Contains(type) && !manager->Empty())
            {
                TypeRow row = manager->Size();
                do
                {
                    row--;
                    helper.Execute(manager, row, lambda);
                } while (row != 0);
            }
        }
    };
};

template <typename... SupportedTypes>
inline Entity<SupportedTypes...> Entity<SupportedTypes...>::invalidEntity;

template <typename... SupportedTypes>
inline Entity<SupportedTypes...> Entity<SupportedTypes...>::entities[EntityCapacity];
