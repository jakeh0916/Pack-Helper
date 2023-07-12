#ifndef PACKHELPER_PACK_ALGORITHMS_H
#define PACKHELPER_PACK_ALGORITHMS_H

// Pack Algorithms [Header]
// Copyright 2023 Jake Huseman

// This file contains a number of algorithms and utility structures for use on the
// packhelp::Pack data structure.
//
// Property Algorithms
//  - At<Index, Pack_t>         -> type
//  - Has<Type_t, Pack_t>       -> bool
//  - Find<Type_t, Pack_t>      -> size_t
// *- Size<Pack_t>              -> size_t
// *- Unique<Pack_t>            -> bool
// *- SubPack<Index, Pack_t>    -> Pack Type
//
// Constructive Algorithms
// *- Reverse<Pack_t>           -> Pack Type
// *- Transform<Pred_t, Pack_t> -> Pack Type
// *- FirstN<Offset, Pack_t>    -> Pack Type
// *- LastN<Offset, Pack_t>     -> Pack Type
// *- Slice<Start, End, Pack_t> -> Pack Type
//
// All algorithms have a struct version and a value version.
//
// Struct version: At_s<Index, Pack_t> {};
// Value version:  At  <Index, Pack_t> = At_s<Index, Pack_t>::Value;
//
// And additionally all algorithms have a "_native" version that work with native
// parameter packs instead of packhelp::Pack structures.
//
// Native struct version: At_native_s<Index, List_ts...> {};
// Native value version:  At_native  <Index, List_ts...> = At_native_s<Index, List_ts...>::Value;


#include "Pack.h"

#include <concepts>
#include <type_traits>

namespace packhelp
{
    // ==================================================================================
    // |                                At and At_native                                |
    // ==================================================================================

    // @brief "At" type trait algorithm.
    // Returns the type at the given index in a Pack.
    // 
    // @tparam Index    -> The index to search for.
    // @tparam Pack_t   -> The Pack to search.
    // @returns Type    -> The returned type.
    template<size_t Index, typename Pack_t>
    struct At_s;

    // @brief Native "At" type trait algorithm.
    // Returns the type at the given index in a template parameter pack.
    // 
    // @tparam Index    -> The index to search for.
    // @tparam List_ts  -> The template parameter pack to search.
    // @returns Type    -> The returned type.
    template<size_t Index, typename... List_ts>
    struct At_native_s;

    template<size_t Index, typename... List_ts>
    struct At_s<Index, Pack<List_ts...>> : At_native_s<Index, List_ts...>
    {
    };

    template<typename Head_t, typename... Tail_ts> // base case: reached index zero
    struct At_native_s<static_cast<size_t>(0), Head_t, Tail_ts...>
    {
        using Type = Head_t;
    };

    template<size_t Index, typename Head_t, typename... Tail_ts> // recursive case
    struct At_native_s<Index, Head_t, Tail_ts...> : At_native_s<Index - static_cast<size_t>(1), Tail_ts...>
    {
        static_assert(Index < Pack<Head_t, Tail_ts...>::Size, "The index is out of bounds for this pack.");
    };

    // @brief "At" type trait algorithm.
    // Returns the type at the given index in a Pack.
    // 
    // @tparam Index    -> The index to search for.
    // @tparam Pack_t   -> The Pack to search.
    template<size_t Index, typename Pack_t>
    using At = At_s<Index, Pack_t>::Type;

    // @brief Native "At" type trait algorithm.
    // Returns the type at the given index in a template parameter pack.
    // 
    // @tparam Index    -> The index to search for.
    // @tparam List_ts  -> The template parameter pack to search.
    template<size_t Index, typename... List_ts>
    using At_native = At_native_s<Index, List_ts...>::Type;

    // ==================================================================================
    // |                               Has and Has_native                               |
    // ==================================================================================

    // @brief "Has" type trait algorithm.
    // Returns whether a given type is contained in a Pack.
    // 
    // @tparam X_t      -> The type to search for.
    // @tparam Pack_t   -> The Pack to search.
    // @returns Value   -> Whether the type was found.
    template<typename X_t, typename Pack_t>
    struct Has_s;

    // @brief Native "Has" type trait algorithm.
    // Returns whether a given type is contained in a template parameter pack.
    // 
    // @tparam X_t      -> The type to search for.
    // @tparam List_ts  -> The template parameter pack to search.
    // @returns Value   -> Whether the type was found.
    template<typename X_t, typename... List_ts>
    struct Has_native_s;

    template<typename X_t, typename... List_ts>
    struct Has_s<X_t, Pack<List_ts...>> : Has_native_s<X_t, List_ts...>
    {
    };

    template<typename X_t> // base case
    struct Has_native_s<X_t>
    {
        static constexpr bool Value = false;
    };

    template<typename X_t, typename Head_t, typename... Tail_ts> // found case
        requires std::same_as<X_t, Head_t>
    struct Has_native_s<X_t, Head_t, Tail_ts...>
    {
        static constexpr bool Value = true;
    };

    template<typename X_t, typename Head_t, typename... Tail_ts> // recursive case
        requires (!std::same_as<X_t, Head_t>)
    struct Has_native_s<X_t, Head_t, Tail_ts...>
        : Has_native_s<X_t, Tail_ts...>
    {
    };

    // @brief "Has" type trait algorithm.
    // Returns whether a given type is contained in a Pack.
    // 
    // @tparam X_t      -> The type to search for.
    // @tparam Pack_t   -> The Pack to search.
    template<typename X_t, typename Pack_t>
    constexpr bool Has = Has_s<X_t, Pack_t>::Value;

    // @brief Native "Has" type trait algorithm.
    // Returns whether a given type is contained in a template parameter pack.
    // 
    // @tparam X_t      -> The type to search for.
    // @tparam List_ts  -> The template parameter pack to search.
    template<typename X_t, typename... List_ts>
    constexpr bool Has_native = Has_native_s<X_t, List_ts...>::Value;

    // ==================================================================================
    // |                              Find and Find_native                              |
    // ==================================================================================

    // @brief Constant value returned if the type was not found.
    constexpr size_t NotFound = static_cast<size_t>(-1);

    // @brief "Find" type trait algorithm.
    // Returns the index where a type first occurs in a Pack.
    // 
    // @tparam X_t      -> The type to search for.
    // @tparam Pack_t   -> The Pack to search.
    // @returns Value   -> The index where X_t first occurs in the pack.
    //                     If X_t isn't found, then NotFound is returned.
    template<typename X_t, typename Pack_t>
    struct Find_s;

    namespace internal
    {
        template<typename X_t, size_t Counter, typename... List_ts>
        struct _Find_native_s;
    }

    // @brief Native "Find" type trait algorithm.
    // Returns the index where a type first occurs in a template paramater pack.
    // 
    // @tparam X_t      -> The type to search for.
    // @tparam List_ts  -> The template paramater pack to search.
    // @returns Value   -> The index where X_t first occurs in the pack.
    //                     If X_t isn't found, then NotFound is returned.
    template<typename X_t, typename... List_ts>
    using Find_native_s = internal::_Find_native_s<X_t, static_cast<size_t>(0), List_ts...>;

    template<typename X_t, typename... List_ts>
    struct Find_s<X_t, Pack<List_ts...>> : Find_native_s<X_t, List_ts...>
    {
    };

    namespace internal
    {
        template<typename X_t, size_t Counter> // base case
        struct _Find_native_s<X_t, Counter>
        {
            static constexpr size_t Value = NotFound;
        };

        template<typename X_t, size_t Counter, typename Head_t, typename... Tail_ts> // found case, non-recursive
            requires std::same_as<X_t, Head_t>
        struct _Find_native_s<X_t, Counter, Head_t, Tail_ts...>
        {
            static constexpr size_t Value = Counter;
        };

        template<typename X_t, size_t Counter, typename Head_t, typename... Tail_ts> // recursive case
            requires (!std::same_as<X_t, Head_t>)
        struct _Find_native_s<X_t, Counter, Head_t, Tail_ts...>
            : _Find_native_s<X_t, Counter + static_cast<size_t>(1), Tail_ts...>
        {
        };
    }

    // @brief "Find" type trait algorithm.
    // Returns the index where a type first occurs in a Pack.
    // 
    // @tparam X_t      -> The type to search for.
    // @tparam Pack_t   -> The Pack to search.
    template<typename X_t, typename Pack_t>
    constexpr size_t Find = Find_s<X_t, Pack_t>::Value;

    // @brief Native "Find" type trait algorithm.
    // Returns the index where a type first occurs in a template paramater pack.
    // 
    // @tparam X_t      -> The type to search for.
    // @tparam List_ts  -> The template paramater pack to search.
    template<typename X_t, typename... List_ts>
    constexpr size_t Find_native = Find_native_s<X_t, List_ts...>::Value;

    // ============================ Unique ============================

    /* Not current working

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

    */
}

#endif // PACKHELPER_PACK_ALGORITHMS_H
