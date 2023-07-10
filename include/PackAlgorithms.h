#ifndef PACKHELPER_PACK_ALGORITHMS_H
#define PACKHELPER_PACK_ALGORITHMS_H

// Pack Algorithms [Header]
// Copyright 2023 Jake Huseman

// This file contains a number of algorithms and utility structures for use on the
// packhelp::Pack data structure.
//
// Property Utilities
// ==================
// These utility structures query certain non-trivial properties of a packhelp::Pack.
// 
// - Has<P, T>               - Returns (through ::Value) whether a pack has a given type.
// - HasAt<P, T>             - Returns (through ::Value) whether a pack has a given type,
//                             and returns (through ::At) the index where that type was
//                             first found.
// - IsUnique<P>             - Returns (through ::Value) whether a pack has all unique types.
// - IsCombination<P0, P1>   - Returns (through ::Value) whether the types in P0 are a 
//                             valid combination of the types in P1.
// - IsPermutation<P0, P1>   - Returns (through ::Value) whether the types in P0 are a
//                             valid permutation of the types in P2.
// - IsSameAs<P0, P1>        - Returns (through ::Value) whether two packs are identical.
// 
// Transformative Utilities
// ========================
// These utility structures perform an operation and provide a modified packhelp::Pack.
// 
// - Transform<P, Pred<P>>   - Returns a pack (through ::Type) for which every type is
//                             transformed using the Predicate<T> template parameter.
// - FirstN<P, N>            - Returns a pack (through ::Type) that contains the first N
//                             types of the input pack.
// - LastN<P, N>             - Returns a pack (through ::Type) that contains the last N
//                             types of the input pack.
// - Slice<P, First, Last>   - Returns a pack (through ::Type) that contains the types
//                             in the input pack from index First to Last.

#include "Pack.h"

#include <concepts>
#include <type_traits>

namespace packhelp
{
    // ============================ Has ============================

    template<typename X_t, typename Pack_t>
    struct Has_t;

    template<typename X_t, typename... List_ts>
    struct Has_native_t;

    template<typename X_t, typename... List_ts>
    struct Has_t<X_t, Pack<List_ts...>> : Has_native_t<X_t, List_ts...>
    {
    };

    // base case
    template<typename X_t>
    struct Has_native_t<X_t>
    {
        static constexpr bool Value = false;
    };

    // found case, non-recursive
    template<typename X_t, typename Head_t, typename... Tail_ts>
        requires std::same_as<X_t, Head_t>
    struct Has_native_t<X_t, Head_t, Tail_ts...>
    {
        static constexpr bool Value = true;
    };

    // recursive case
    template<typename X_t, typename Head_t, typename... Tail_ts>
        requires (!std::same_as<X_t, Head_t>)
    struct Has_native_t<X_t, Head_t, Tail_ts...>
        : Has_native_t<X_t, Tail_ts...>
    {
    };

    // ---------------------- Has - value ----------------------

    template<typename X_t, typename Pack_t>
    constexpr bool Has = Has_t<X_t, Pack_t>::Value;

    template<typename X_t, typename... List_ts>
    constexpr bool Has_native = Has_native_t<X_t, List_ts...>::Value;

    // ============================ Find ============================

    constexpr size_t NotFound = static_cast<size_t>(-1);

    template<typename X_t, typename Pack_t>
    struct Find_t;

    namespace internal
    {
        template<typename X_t, size_t Counter, typename... List_ts>
        struct _Find_native_t;
    }

    template<typename X_t, typename... List_ts>
    using Find_native_t = internal::_Find_native_t<X_t, static_cast<size_t>(0), List_ts...>;

    template<typename X_t, typename... List_ts>
    struct Find_t<X_t, Pack<List_ts...>> : Find_native_t<X_t, List_ts...>
    {
    };

    namespace internal
    {
        // base case
        template<typename X_t, size_t Counter>
        struct _Find_native_t<X_t, Counter>
        {
            static constexpr size_t Value = NotFound;
        };

        // found case, non-recursive
        template<typename X_t, size_t Counter, typename Head_t, typename... Tail_ts>
            requires std::same_as<X_t, Head_t>
        struct _Find_native_t<X_t, Counter, Head_t, Tail_ts...>
        {
            static constexpr size_t Value = Counter;
        };

        // recursive case
        template<typename X_t, size_t Counter, typename Head_t, typename... Tail_ts>
            requires (!std::same_as<X_t, Head_t>)
        struct _Find_native_t<X_t, Counter, Head_t, Tail_ts...>
            : _Find_native_t<X_t, Counter + static_cast<size_t>(1), Tail_ts...>
        {
        };
    }

    // ---------------------- Find - value ----------------------

    template<typename X_t, typename Pack_t>
    constexpr size_t Find = Find_t<X_t, Pack_t>::Value;

    template<typename X_t, typename... List_ts>
    constexpr size_t Find_native = Find_native_t<X_t, List_ts...>::Value;

    // ============================ Unique ============================

    template<typename Pack_t>
    struct IsUnique_t;

    template<typename... List_ts>
    struct IsUnique_native_t;

    template<typename... List_ts>
    struct IsUnique_t<Pack<List_ts...>> : IsUnique_native_t<List_ts...>
    {
    };

    // recursive case
    template<typename A_t, typename B_t, typename ... Tail_ts>
    struct IsUnique_native_t<A_t, B_t, Tail_ts...>
        : IsUnique_native_t<A_t, B_t>, IsUnique_native_t<A_t, Tail_ts...>, IsUnique_native_t<B_t, Tail_ts...>
    {
        static constexpr bool Value = 
            IsUnique_native_t<A_t, B_t>::Value
            && IsUnique_native_t<A_t, Tail_ts...>::Value
            && IsUnique_native_t<B_t, Tail_ts...>::Value;
    };

    // base case 1: not unique
    template <class A_t, class B_t> requires std::same_as<A_t, B_t>
    struct IsUnique_native_t<A_t, B_t>
    {
        static constexpr bool Value = false;
    };

    // base case 2: unique
    template <class A_t, class B_t> requires (!std::same_as<A_t, B_t>)
        struct IsUnique_native_t<A_t, B_t>
    {
        static constexpr Value = true;
    };

    // ------------------ unique - value ------------------

    template<typename Pack_t>
    constexpr bool IsUnique = IsUnique_t<Pack_t>::Value;

    template<typename... List_ts>
    constexpr bool IsUnique_native = IsUnique_native_t<List_ts...>::Value;
}

#endif // PACKHELPER_PACK_ALGORITHMS_H
