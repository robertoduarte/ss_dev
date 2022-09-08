#pragma once

#include "..\Utils\TemplateUtils.hpp"
#include <stddef.h>

template <typename... SupportedTypes>
class ArchetypeId
{
private:
    template <typename T>
    static constexpr size_t Order()
    {
        size_t id = 0;
        ((std::is_same_v<T, SupportedTypes> ? false : (id++, true)) && ...);
        return id;
    }

    static constexpr size_t ComponentIndexFromName(const char *name)
    {
        size_t id = 0;
        ((MatchesTypeName<SupportedTypes>(name) ? (id = ComponentIndex<SupportedTypes>(), false) : true) && ...);
        return id;
    }

    uint64_t id;

public:
    // Type Utils
    template <typename Type>
    static constexpr size_t ComponentIndex()
    {
        static_assert((... || std::is_same_v<Type, SupportedTypes>),
                      "Type not supported by ArchetypeId");

        size_t biggerCnt = ((!std::is_same_v<Type, SupportedTypes> && sizeof(SupportedTypes) > sizeof(Type)) + ...);
        size_t sameSizeCnt = ((Order<SupportedTypes>() < Order<Type>() && sizeof(SupportedTypes) == sizeof(Type)) + ...);

        return biggerCnt + sameSizeCnt;
    }

    static constexpr size_t NonTagComponentCount()
    {
        return ((!std::is_empty_v<SupportedTypes>)+...);
    }

    // Constructors
    constexpr ArchetypeId(const char *typeName) : id(1 << ComponentIndexFromName(typeName)){};
    constexpr ArchetypeId(const ArchetypeId &a) : id(a.id){};
    constexpr ArchetypeId() : id(0){};

    template <typename... T>
    static constexpr ArchetypeId ConstArchetypeId()
    {
        ArchetypeId a;
        if constexpr (sizeof...(T))
            a.id = (... | (1 << ComponentIndex<T>()));
        return a;
    }

    // Functions
    constexpr size_t Size() const
    {
        return ((!std::is_empty_v<SupportedTypes> && Contains<SupportedTypes>() ? sizeof(SupportedTypes) : 0) + ...);
    }

    constexpr bool Contains(const ArchetypeId &a) const { return (id & a.id) == a.id; }

    constexpr bool Empty() const { return id == 0; }

    template <typename T>
    constexpr bool Contains() const { return id & (1 << ComponentIndex<T>()); }

    template <typename T>
    constexpr size_t GetOffset() const
    {
        return ((ComponentIndex<SupportedTypes>() < ComponentIndex<T>() && Contains<SupportedTypes>() ? sizeof(SupportedTypes) : 0) + ...);
    }

    // Operators
    constexpr ArchetypeId &operator+=(const ArchetypeId &a)
    {
        id |= a.id;
        return *this;
    }

    constexpr ArchetypeId &operator-=(const ArchetypeId &a)
    {
        id &= ~a.id;
        return *this;
    }

    constexpr ArchetypeId operator+(const ArchetypeId &a) const { return ArchetypeId(*this) += a; }
    constexpr ArchetypeId operator-(const ArchetypeId &a) const { return ArchetypeId(*this) -= a; }
    constexpr bool operator==(const ArchetypeId &a) const { return id == a.id; }
    constexpr bool operator!=(const ArchetypeId &a) const { return id != a.id; }
    constexpr bool operator>(const ArchetypeId &a) const { return id > a.id; }
    constexpr bool operator<(const ArchetypeId &a) const { return id < a.id; }
};
