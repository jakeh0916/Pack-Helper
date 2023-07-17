# Pack Helper
A lightweight header-only library that simplifies the usage/analysis of template parameter packs in C++20 and beyond. Created by Jake Huseman.

# Documentation

### Creating a Pack
A `Pack` is a compile-time container for any number of types. To create a pack, define a new type using `Pack` and your list of types:

```cpp
using IntegralTypesPack = Pack<char, short, int, long, long long>;
```

#### Using Traits

To use Pack Helper for static type reflection, include the `Traits.h` header. This file contains a number of useful type trait structures for the `Pack`. Here are a few examples:

Use `Size` to determine the size of a pack.
```cpp
constexpr size_t Count = Size<IntegralTypesPack>::Value; // Count == 5.
```

Use `Find` to find a type within a pack, or determine that it's absent.
```cpp
constexpr bool IndexA = Find<int, IntegralTypesPack>::Value;  // IndexA == 2.
constexpr bool IndexB = Find<float, IntegralTypesPack>>Value; // IndexB == NotFound (-1).
```

Use `Unique` to determine if a pack has all unique types.
```cpp
constexpr bool IsUnique = Unique<IntegralTypesPack>::Value; // IsUnique == true.
```

See the `Traits.h` file for a full list of supported type traits.
