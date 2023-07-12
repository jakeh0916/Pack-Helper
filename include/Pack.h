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
//  ----------------------------        ----------------------        ------------------- 
// | Pack<int, double, char*>   |      | Pack<double, char*>  |      | Pack<char*>       |
// |----------------------------|      |----------------------|      |-------------------|
// | Type = int                 |      | Type = double        |      | Type = char*      |
// | Base = Pack<double, char*> |  ->  | Base = Pack<char*>   |  ->  | Base = Pack<>     |
//  ----------------------------        ----------------------        ------------------- 
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

    // base case: empty pack
    template<>
    struct Pack<>
    {
        // @brief Gets the size of the Pack.
        static constexpr size_t Size = static_cast<size_t>(0);
    };

    // recursive case
    template<typename Type_t, typename... Rest_ts>
    struct Pack<Type_t, Rest_ts...> : Pack<Rest_ts...>
    {
        using Type = typename Type_t;
        using Base = typename Pack<Rest_ts...>;

        // @brief Gets the size of the Pack.
        static constexpr size_t Size = sizeof...(Rest_ts) + static_cast<size_t>(1);
    };
};

#endif // PACKHELPER_PACK_H
