#ifndef PACKHELPER_PACK_H
#define PACKHELPER_PACK_H

// Pack [Header]
// Copyright 2023 Jake Huseman

// This file contains the packhelp::Pack and packhelp::PackElement types which are simple
// data structures that can hold and access a template parameter pack. 
// 
// The packhelp::Pack structure holds a list of types using recursive inheritance. This can
// be visualized as a chain of related Pack<...> types as shown below. (For more detail on
// recursive inheritance, see the implementation of std::tuple.)
// 
// |----------------------------|      |----------------------|      |-------------------|
// | Pack<int, double, char*>   |      | Pack<double, char*>  |      | Pack<char*>       |
// |----------------------------|      |----------------------|      |-------------------|
// | Type = int                 |      | Type = double        |      | Type = char*      |
// | Base = Pack<double, char*> |  ->  | Base = Pack<char*>   |  ->  | Base = Pack<>     |
// |----------------------------|      |----------------------|      |-------------------|
// 
// The packhelp::PackElement allows access to the Nth type in a packhelp::Pack. This can also
// be achieved with the TypeAt, and PackAt type definitions on each packhelp::Pack type. For
// example, the types A, B, C can be accessed in the following ways:
// 
// 1. using P = Pack<A, B, C>;
// 2. using A_type = Pack::Type;       // For the first type, we can just use Pack<...>::Type.
// 3. using B_type = Pack::Base::Type; // For the second type, we can simply get the Base's type.
// 4. using C_type = Pack::TypeAt<2>;  // For types deeper in the list, we can use TypeAt<N>
//                                     // to get a specified type. This works using PackElement.

namespace packhelp
{
    template<typename... Ts>
    struct Pack;

    // @brief The PackElement structure - allows access to the Nth type/sub-pack in a given pack.
    // @tparam N - The index to search for.
    // @tparam P - The given pack.
    template<size_t N, typename P>
    struct PackElement;

    template<size_t N> // Base case A (empty pack)
    struct PackElement<N, Pack<>>
    {
    };

    template<typename T, typename... R> // Base case B (index zero)
    struct PackElement<static_cast<size_t>(0), Pack<T, R...>>
    {
        using CurrentPack = typename Pack<T, R...>;
        using CurrentType = typename T;
    };

    template<size_t N, typename T, typename... R> // Recursive case
    struct PackElement<N, Pack<T, R...>> : PackElement<N - static_cast<size_t>(1), Pack<R...>>
    {
        static_assert(N < Pack<T, R...>::Size, "The index N is out of bounds for this Pack.");
    };

    // @brief The Pack structure - holds a number of types.
    // @tparam Ts - The list of types to be stored.
    template<typename... Ts>
    struct Pack;

    template<> // Base case: an empty pack.
    struct Pack<>
    {
        using Type = void;

        static constexpr size_t Size = static_cast<size_t>(0);
    };

    template<typename T, typename... R> // Recursive case: a pack with one or more types.
    struct Pack<T, R...> : Pack<R...>
    {
        using Type = typename T;

        using Base = typename Pack<R...>;
        using BaseType = typename Base::Type;

        // @brief Gets the type at index N in this pack.
        // @tparam N - The index to search for.
        template<size_t N>
        using TypeAt = PackElement<N, Pack<T, R...>>::CurrentType;

        // @brief Gets the sub-pack at index N in this pack.
        // @tparam N - The index to search for.
        template<size_t N>
        using PackAt = PackElement<N, Pack<T, R...>>::CurrentPack;

        // @brief Gets the size of the Pack.
        static constexpr size_t Size = sizeof...(R) + static_cast<size_t>(1);
    };
};

#endif // PACKHELPER_PACK_H
