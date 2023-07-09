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

namespace packhelp
{
    template<typename P, typename T>
    struct Has;

    template<typename T>
    struct Has<Pack<>, T>
    {
        static constexpr bool Value = false;
    };

    template<typename T, typename Head, typename... Tail> requires std::same_as<T, Head>
    struct Has<Pack<Head, Tail...>, T> // no inheritance - value is found.
    {
        static constexpr bool Value = true;
    };

    template<typename T, typename Head, typename... Tail> requires (!std::same_as<T, Head>)
    struct Has<Pack<Head, Tail...>, T> : Has<Pack<Tail...>, T>
    {
    };

    template<typename P, typename T, size_t _Counter = static_cast<size_t>(0)>
    struct HasAt;

    template<typename T, size_t _Counter>
    struct HasAt<Pack<>, T, _Counter>
    {
        static constexpr bool Value = false;
    };

    template<typename T, size_t _Counter, typename Head, typename... Tail> requires std::same_as<T, Head>
    struct HasAt<Pack<Head, Tail...>, T, _Counter> // no inheritance - value is found.
    {
        static constexpr bool Value = true;
        static constexpr size_t At  = _Counter;
    };

    template<typename T, size_t _Counter, typename Head, typename... Tail> requires (!std::same_as<T, Head>)
        struct HasAt<Pack<Head, Tail...>, T, _Counter> : HasAt<Pack<Tail...>, T, _Counter + static_cast<size_t>(1)>
    {
    };
}

#endif // PACKHELPER_PACK_ALGORITHMS_H
