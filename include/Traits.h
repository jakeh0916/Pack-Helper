#ifndef PACKHELPER_TRAITS_H
#define PACKHELPER_TRAITS_H

// ================================
// | PackHelper - Traits [Header] |
// | Copyright 2023 Jake Huseman  |
// ================================

// This file contains type-trait structures and constant expressions that can be used to get
// basic information about a pack. The following traits are supported:
// 
// ============================================================================================
// | Trait  | Signature          | Description                                                |
// ============================================================================================
// | IsPack | IsPack<Pack>       | Determines whether the type is a packhelp::Pack.           |
// | Size   | Size<Pack>         | Gets the size of the Pack.                                 |
// | At     | At<Index, Pack>    | Retrieves the type at Index in the Pack.                   |
// | Has    | Has<Type, Pack>    | Determines whether the Pack contains Type.                 |
// | Find   | Find<Type, Pack>   | Finds the index where Type first appears in the Pack.      |
// | Unique | Unique<Pack>       | Determines whether the Pack contains only unique types.    |
// ============================================================================================
// 
// Use ::Value or ::Type to get the type trait structure's output. Constant expression versions
// are also available and have a "_v" or "_t" suffix depending on the output. (For example, use
// Size_v to get the size of a pack, or At_t to get the type at a given index.)
// 
// Native versions of each type trait are also provided which allow inline template parameter pack.

#include "Pack.h"

#include <type_traits>
#include <concepts>

namespace packhelp
{
    constexpr size_t NotFound = static_cast<size_t>(-1);

    // ========================================================================================
    // |                                        IsPack                                        |
    // ========================================================================================

    // @brief Pack helper "IsPack" type trait.
    // Determines whether the type is a packhelp::Pack.
    //
    // @tparam Pack - The pack.
    // @return Value - Whether the pack parameter was a packhelp::Pack.
    template<typename Pack>
    struct IsPack;

    template<typename... PackTypes>
    struct IsPack<Pack<PackTypes...>>
    {
        static constexpr bool Value = true;
    };

    template<typename Pack>
    struct IsPack
    {
        static constexpr bool Value = false;
    };

    // @brief Pack helper "IsPack" type trait.
    // Determines whether the type is a packhelp::Pack.
    //
    // @tparam Pack - The pack.
    template<typename Pack>
    constexpr bool IsPack_v = IsPack<Pack>::Value;

    // ========================================================================================
    // |                                Size (and Size_native)                                |
    // ========================================================================================

    // @brief Pack helper "Size" type trait.
    // Gets the size of the Pack.
    //
    // @tparam Pack - The pack.
    // @return Value - The size of the pack.
    template<typename Pack>
    struct Size;

    // @brief Pack helper "Size_native" type trait.
    // Gets the size of the template parameter pack.
    //
    // @tparam PackTypes... - The template parameter pack.
    // @return Value - The size of the template parameter pack.
    template<typename... PackTypes>
    struct Size_native;

    template<typename... PackTypes>
    struct Size<Pack<PackTypes...>> : Size_native<PackTypes...>
    {
    };

    template<typename... PackTypes>
    struct Size_native
    {
        static constexpr size_t Value = sizeof...(PackTypes);
    };

    // @brief Pack helper "Size_v" type trait.
    // Gets the size of the Pack.
    //
    // @tparam Pack - The pack.
    template<typename Pack>
    constexpr size_t Size_v = Size<Pack>::Value;

    // @brief Pack helper "Size_native_v" type trait.
    // Gets the size of the template parameter pack.
    //
    // @tparam PackTypes... - The template parameter pack.
    template<typename... PackTypes>
    constexpr size_t Size_native_v = Size_native<PackTypes...>::Value;

    // ========================================================================================
    // |                                  At (and At_native)                                  |
    // ========================================================================================

    // @brief Pack helper "At" type trait.
    // Retrieves the type at Index in the Pack.
    //
    // @tparam Index - The index to look for.
    // @tparam Pack - The pack.
    // @return Type - The found type.
    template<size_t Index, typename Pack>
    struct At;

    // @brief Pack helper "At_native" type trait.
    // Retrieves the type at Index in the template parameter pack.
    //
    // @tparam Index - The index to look for.
    // @tparam PackTypes... - The template parameter pack.
    // @return Type - The found type.
    template<size_t Index, typename... PackTypes>
    struct At_native;

    template<size_t Index, typename... PackTypes>
    struct At<Index, Pack<PackTypes...>> : At_native<Index, PackTypes...>
    {
    };

    template<size_t Index> // (base case: out of bounds)
    struct At_native<Index>
    {
        static_assert(Index == NotFound, "At_native Error: The index is out of bounds for this pack.");
    };

    template<typename Head, typename... Tail> // (base case: found)
    struct At_native<static_cast<size_t>(0), Head, Tail...>
    {
        using Type = Head;
    };

    template<size_t Index, typename Head, typename... Tail> // (recursive case)
    struct At_native<Index, Head, Tail...> : At_native<Index - static_cast<size_t>(1), Tail...>
    {
    };

    // @brief Pack helper "At_t" type trait.
    // Retrieves the type at Index in the Pack.
    //
    // @tparam Index - The index to look for.
    // @tparam Pack - The pack.
    template<size_t Index, typename Pack>
    using At_t = At<Index, Pack>::Type;

    // @brief Pack helper "At_native_t" type trait.
    // Retrieves the type at Index in the template parameter pack.
    //
    // @tparam Index - The index to look for.
    // @tparam PackTypes... - The template parameter pack.
    template<size_t Index, typename... PackTypes>
    using At_native_t = At_native<Index, PackTypes...>::Type;

    // ========================================================================================
    // |                                 Has (and Has_native)                                 |
    // ========================================================================================

    // @brief Pack helper "Has" type trait.
    // Determines whether the Pack contains Type.
    //
    // @tparam Type - The type to look for.
    // @tparam Pack - The pack.
    // @return Value - Whether the type was found.
    template<typename Type, typename Pack>
    struct Has;

    // @brief Pack helper "Has_native" type trait.
    // Determines whether the template parameter pack contains Type.
    //
    // @tparam Type - The type to look for.
    // @tparam PackTypes... - The template parameter pack.
    // @return Value - Whether the type was found.
    template<typename Type, typename... PackTypes>
    struct Has_native;

    template<typename Type, typename... PackTypes>
    struct Has<Type, Pack<PackTypes...>> : Has_native<Type, PackTypes...>
    {
    };

    template<typename Type> // (base case: not found)
    struct Has_native<Type>
    {
        static constexpr bool Value = false;
    };

    template<typename Type, typename Head, typename... Tail> // (base case: found)
        requires std::same_as<Type, Head>
    struct Has_native<Type, Head, Tail...>
    {
        static constexpr bool Value = true;
    };

    template<typename Type, typename Head, typename... Tail> // (recursive case)
        requires (!std::same_as<Type, Head>)
    struct Has_native<Type, Head, Tail...> : Has_native<Type, Tail...>
    {
    };

    // @brief Pack helper "Has_v" type trait.
    // Determines whether the Pack contains Type.
    //
    // @tparam Type - The type to look for.
    // @tparam Pack - The pack.
    template<typename Type, typename Pack>
    constexpr bool Has_v = Has<Type, Pack>::Value;

    // @brief Pack helper "Has_native_v" type trait.
    // Determines whether the template parameter pack contains Type.
    //
    // @tparam Type - The type to look for.
    // @tparam PackTypes... - The template parameter pack.
    template<typename Type, typename... PackTypes>
    constexpr bool Has_native_v = Has_native<Type, PackTypes...>::Value;

    // ========================================================================================
    // |                                Find (and Find_native)                                |
    // ========================================================================================

    // @brief Pack helper "Find" type trait.
    // Finds the index where Type first appears in the Pack.
    //
    // @tparam Type - The type to look for.
    // @tparam Pack - The pack.
    // @return Value - Index where the type is first found.
    template<typename Type, typename Pack>
    struct Find;

    namespace internal
    {
        template<size_t Counter, typename Type, typename... PackTypes>
        struct _Find_native;
    }

    // @brief Pack helper "Find_native" type trait.
    // Finds the index where Type first appears in the template parameter pack.
    //
    // @tparam Type - The type to look for.
    // @tparam PackTypes... - The template parameter pack.
    // @return Value - Index where the type is first found.
    template<typename Type, typename... PackTypes>
    using Find_native = internal::_Find_native<static_cast<size_t>(0), Type, PackTypes...>;

    template<typename Type, typename... PackTypes>
    struct Find<Type, Pack<PackTypes...>> : Find_native<Type, PackTypes...>
    {
    };

    namespace internal
    {
        template<size_t Counter, typename Type> // (base case: not found)
        struct _Find_native<Counter, Type>
        {
            static constexpr size_t Value = NotFound;
        };

        template<size_t Counter, typename Type, typename Head, typename... Tail> // (base case: found)
            requires std::same_as<Type, Head>
        struct _Find_native<Counter, Type, Head, Tail...>
        {
            static constexpr size_t Value = Counter;
        };

        template<size_t Counter, typename Type, typename Head, typename... Tail> // (recursive case)
            requires (!std::same_as<Type, Head>)
        struct _Find_native<Counter, Type, Head, Tail...>
            : _Find_native<Counter + static_cast<size_t>(1), Type, Tail...>
        {
        };
    }

    // @brief Pack helper "Find_v" type trait.
    // Finds the index where Type first appears in the Pack.
    //
    // @tparam Type - The type to look for.
    // @tparam Pack - The pack.
    template<typename Type, typename Pack>
    constexpr size_t Find_v = Find<Type, Pack>::Value;

    // @brief Pack helper "Find_native_v" type trait.
    // Finds the index where Type first appears in the template parameter pack.
    //
    // @tparam Type - The type to look for.
    // @tparam PackTypes... - The template parameter pack.
    template<typename Type, typename... PackTypes>
    constexpr size_t Find_native_v = internal::_Find_native<static_cast<size_t>(0), Type, PackTypes...>::Value;

    // ========================================================================================
    // |                              Unique (and Unique_native)                              |
    // ========================================================================================

    // @brief Pack helper "Unique" type trait.
    // Determines whether the Pack contains only unique types.
    //
    // @tparam Pack - The pack.
    // @return Value - Whether all types in the pack are unique.
    template<typename Pack>
    struct Unique;

    // @brief Pack helper "Unique_native" type trait.
    // Determines whether the template parameter pack contains only unique types.
    //
    // @tparam Pack - The template parameter pack.
    // @return Value - Whether all types in the pack are unique.
    template<typename... PackTypes>
    struct Unique_native;

    template<typename... PackTypes>
    struct Unique<Pack<PackTypes...>> : Unique_native<PackTypes...>
    {
    };

    template<> // (base case: unique)
    struct Unique_native<>
    {
        static constexpr bool Value = true;
    };

    template<typename Type> // (base case: unique)
    struct Unique_native<Type>
    {
        static constexpr bool Value = true;
    };

    template<typename TypeA, typename TypeB, typename... Rest> // (base case: not unique)
        requires std::same_as<TypeA, TypeB>
    struct Unique_native<TypeA, TypeB, Rest...>
    {
        static constexpr bool Value = false;
    };

    template<typename TypeA, typename TypeB, typename... Rest> // (recursive case)
        requires (!std::same_as<TypeA, TypeB>)
    struct Unique_native<TypeA, TypeB, Rest...>
        : Unique_native<TypeA, Rest...>
        , Unique_native<TypeB, Rest...>
    {
        static constexpr bool Value =
            Unique_native<TypeA, Rest...>::Value && Unique_native<TypeB, Rest...>::Value;
    };

    // @brief Pack helper "Unique_v" type trait.
    // Determines whether the Pack contains only unique types.
    //
    // @tparam Pack - The pack.
    template<typename Pack>
    constexpr bool Unique_v = Unique<Pack>::Value;

    // @brief Pack helper "Unique_native_v" type trait.
    // Determines whether the template parameter pack contains only unique types.
    //
    // @tparam Pack - The template parameter pack.
    template<typename... PackTypes>
    constexpr bool Unique_native_v = Unique_native<PackTypes...>::Value;
}

#endif // PACKHELPER_TRAITS_H
