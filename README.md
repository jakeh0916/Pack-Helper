## Pack Helper
A lightweight C++ library that simplifies the usage/analysis of template paramter packs.

Created by Jake Huseman - Copyright 2023

## Library Usage
Pack Helper is a small header-only library. To use, simply copy the `include` directory into your project's `include/PackHelper/`. All library structures and functions are located in the `packhelp` namespace.


#### Creating a New Pack
Create a packhelp::Pack from a template parameter pack (requires C++11 and later). The following will create a new Pack containing an integer, a double reference, and a standard string.
```cpp
using MyPack = packhelp::Pack<int, double&, std::string>;
```
#### Using Pack Algorithms
Use the algorithms in PackAlgorithms.h to query non-trivial properties of a packhelp::Pack or construct new packs via transformative functions.

The `Has` and `HasAt` traits can check if a Pack `P` contains a type `T`. `HasAt` can also provide the index where this type first appears in the Pack.
```cpp
static_assert(packhelp::Has<MyPack, int>::Value        == true);
static_assert(packhelp::HasAt<MyPack, std::string>::At == 2);
```

The `IsUnique` trait determines whether a Pack's types are all unique.
```cpp
static_assert(packhelp::IsUnique<MyPack>::Value == true);
```

Both `IsCombination` and `IsPermutation` take two parameter packs as input, and respectively determine whether the first Pack's types are a valid combination or permutation of the second Pack's types.
```cpp
using MyOtherPack1 = packhelp::Pack<double&, int>;
using MyOtherPack2 = packhelp::Pack<double&, std::string, int>;

// Both MyOtherPack1 and MyOtherPack2 are valid combinations of MyPack.
static_assert(packhelp::IsCombination<MyOtherPack1, MyPack> == true);
static_assert(packhelp::IsCombination<MyOtherPack2, MyPack> == true);

// MyOtherPack1 is not a permutation of MyPack, but MyOtherPack2 is.
static_assert(packhelp::IsPermutation<MyOtherPack1, MyPack> == false);
static_assert(packhelp::IsPermutation<MyOtherPack2, MyPack> == true);
```

