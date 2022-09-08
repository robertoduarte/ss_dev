#pragma once

#include "std\type_traits.h"

template <size_t... Ints>
struct index_sequence
{
    using type = index_sequence;
    using value_type = size_t;
    static constexpr size_t size() noexcept { return sizeof...(Ints); }
};

template <class Sequence1, class Sequence2>
struct _merge_and_renumber;

template <size_t... I1, size_t... I2>
struct _merge_and_renumber<index_sequence<I1...>, index_sequence<I2...>>
    : index_sequence<I1..., (sizeof...(I1) + I2)...>
{
};

template <size_t N>
struct make_index_sequence
    : _merge_and_renumber<typename make_index_sequence<N / 2>::type,
                          typename make_index_sequence<N - N / 2>::type>
{
};

template <>
struct make_index_sequence<0> : index_sequence<>
{
};
template <>
struct make_index_sequence<1> : index_sequence<0>
{
};

template <size_t... i>
constexpr bool StrCompare(const char *a, const char *b, index_sequence<i...>)
{
    return ((a[i] == b[i]) && ...);
}

template <size_t... i>
constexpr size_t FindLastToken(const char *a, const char token, index_sequence<i...> indexes)
{
    size_t lastTokenOccurence = indexes.size();
    ((a[i] == token ? (lastTokenOccurence = i, true) : true) && ...);
    return lastTokenOccurence;
}

template <typename T>
constexpr bool MatchesTypeName(const char *str)
{
    constexpr size_t prettyFunctionSize = sizeof(__PRETTY_FUNCTION__);
    constexpr auto begin = FindLastToken(__PRETTY_FUNCTION__, ' ', make_index_sequence<prettyFunctionSize>{}) + 1;
    static_assert(begin != prettyFunctionSize, "Failed to get position of type name text.");
    constexpr size_t size = prettyFunctionSize - begin - 1;
    constexpr const char *name = __PRETTY_FUNCTION__ + begin;
    return StrCompare(name, str, make_index_sequence<size - 1>{});
}

template <typename T>
inline const char *TypeName()
{
    constexpr size_t prettyFunctionSize = sizeof(__PRETTY_FUNCTION__);
    constexpr size_t begin = FindLastToken(__PRETTY_FUNCTION__, ' ', make_index_sequence<prettyFunctionSize>{}) + 1;
    static_assert(begin != prettyFunctionSize, "Failed to get position of type name text.");
    constexpr size_t size = prettyFunctionSize - begin - 1;
    static char typeName[size] = {};
    memcpy(typeName, __PRETTY_FUNCTION__ + begin, size - 1);
    return typeName;
}

template <size_t A, size_t B>
struct PowerOf
{
    static constexpr size_t value = A * PowerOf<A, B - 1>::value;
};

template <size_t A>
struct PowerOf<A, 0>
{
    static constexpr size_t value = 1;
};