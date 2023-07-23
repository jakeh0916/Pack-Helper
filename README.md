# Pack Helper

A lightweight header-only library that simplifies the usage/analysis of template parameter packs in C++20 and beyond. Created by Jake Huseman.

# Documentation

### Using the Library

Include `PackHelper.h` in at least one C++ source file.

To run all Pack Helper testcases during each build, `#define` the `PACKHELPER__DO_TESTS` token.

To include Pack Helper without any compiler warnings/errors, `#define` the `PACKHELPER__QUIET` token.

### Creating a Pack

A `Pack` is a compile-time container for any number of types. To create a pack, define a new type using `Pack` and your list of types:

```cpp
using IntegralTypesPack = Pack<char, short, int, long, long long>;
```

Packs use recursive inheritance to store their types. To get a Pack's first type, you can use `Pack::Type` (for the above example, this would be `char`). `Pack::Base` will gave you a Pack containing the remaining types (for the example, this would be `Pack<short, int, long, long long>`).

You can think of a Pack as a chain of derived types where each subsequent Pack's `Type` is a type in the template parameter pack sequence. (See the implementation of `std::tuple` for more information on this technique.)

```
Diagram of the PackHelper 'Pack' data structure:

===========================       ======================       ==================       ===========
| Pack<short, int, long>  |       | Pack<int, long>    |       | Pack<long>     |       | Pack<>  |
|-------------------------|       |--------------------|       |----------------|       |---------|
| Type = short            |       | Type = int         |       | Type = long    |       |         |
| Base = Pack<int, long>  |  -|>  | Base = Pack<long>  |  -|>  | Base = Pack<>  |  -|>  |         |
===========================       ======================       ==================       ===========
```

#### Using Traits

To use Pack Helper for static type reflection, include the `Traits.h` header. This file contains a number of useful type trait structures for the `Pack`. Here are a couple of examples:

Use `Find` to find a type within a pack, or determine that it's absent.
```cpp
constexpr bool IndexA = Find<int, IntegralTypesPack>::Value;  // IndexA == 2.
constexpr bool IndexB = Find<float, IntegralTypesPack>::Value; // IndexB == NotFound (-1).
```

Use `Unique` to determine if a pack has all unique types.
```cpp
constexpr bool IsUnique = Unique<IntegralTypesPack>::Value; // IsUnique == true.
```

See the `Traits.h` file for a full list of supported type traits.
