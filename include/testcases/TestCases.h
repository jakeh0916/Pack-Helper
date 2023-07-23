#ifndef PACKHELPER_TEST_CASES_H
#define PACKHELPER_TEST_CASES_H

// ====================================
// | PackHelper - Test Cases [Header] |
// | Copyright 2023 Jake Huseman      |
// ====================================

// Since Packs and Pack type traits are always deduced at compile time, all test cases
// are in the form of a static_assert.
// 
// The TestFailureID macro is used to identify a test category and a test number, and
// the TestFailureDescription gives information about the specifics of the test case.
// (This information is then printed to the compiler output if a test is failed.)

#include "../core/Pack.h"
#include "../core/Traits.h"

#define TestFailureID(Category, Number) "** Test Case Failure ** Failed test case '" Category "' (#" #Number ")"
#define TestFailureDescription(Description) " (Test Case Description: " Description ")"

namespace packhelp
{
    namespace internal
    {
        // "IsPack" Test Cases
        // ===================

        static_assert(
            IsPack<Pack<>>::Value == true,

            TestFailureID("IsPack", 1)
            TestFailureDescription("An empty pack should be a pack.")
            );

        static_assert(
            IsPack<Pack<char, short, int, long>>::Value == true,

            TestFailureID("IsPack", 2)
            TestFailureDescription("An pack of four types should be a pack.")
            );

        static_assert(
            IsPack<int>::Value == false,

            TestFailureID("IsPack", 3)
            TestFailureDescription("An int should not be a pack.")
            );

        template<typename... Types>
        struct NotAPack
        {
        };

        static_assert(
            IsPack<NotAPack<char, short, int, long>>::Value == false,

            TestFailureID("IsPack", 4)
            TestFailureDescription("Another variadic templated class should not be a pack.")
            );

        // "Size" Test Cases
        // =================

        static_assert(
            Size<Pack<>>::Value == 0 &&
            Size_v<Pack<>> == 0 &&
            Size_native<>::Value == 0 &&
            Size_native_v<> == 0,

            TestFailureID("Size", 1)
            TestFailureDescription("The size of an empty pack should be zero.")
            );

        static_assert(
            Size<Pack<int, int&, int&&, double, double&, double&&>>::Value == 6 &&
            Size_v<Pack<int, int&, int&&, double, double&, double&&>> == 6 &&
            Size_native<int, int&, int&&, double, double&, double&&>::Value == 6 &&
            Size_native_v<int, int&, int&&, double, double&, double&&> == 6,

            TestFailureID("Size", 2)
            TestFailureDescription("The size of the example pack should be 6 and equal for all type trait versions.")
            );

        // "At" Test Cases
        // ===============

        static_assert(
            std::is_same_v<
            At<0, Pack<int, int&, int&&>>::Type,
            int>
            && std::is_same_v<
            At_t<0, Pack<int, int&, int&&>>,
            int>
            && std::is_same_v<
            At_native<0, int, int&, int&&>::Type,
            int>
            && std::is_same_v<
            At_native_t<0, int, int&, int&&>,
            int>,

            TestFailureID("At", 1)
            TestFailureDescription("The type at position 0 in the given pack should be int and equal for all type trait versions.")
            );

        static_assert(
            std::is_same_v<
            At<0, Pack<int, int&, int&&>>::Type,
            int>
            && std::is_same_v<
            At<1, Pack<int, int&, int&&>>::Type,
            int&>
            && std::is_same_v<
            At<2, Pack<int, int&, int&&>>::Type,
            int&&>,

            TestFailureID("At", 2)
            TestFailureDescription("The types at position 0, 1, 2 in the given pack should be int, int&, int&.")
            );

        // "Has" Test Cases
        // ================

        static_assert(
            Has<void, Pack<>>::Value == false,

            TestFailureID("Has", 1)
            TestFailureDescription("An empty pack should not 'have' any types.")
            );

        static_assert(
            Has<int, Pack<int>>::Value == true
            && Has_v<int, Pack<int>> == true
            && Has_native<int, int>::Value == true
            && Has_native_v<int, int> == true,

            TestFailureID("Has", 2)
            TestFailureDescription("The given pack should contain 'int' and be equal for all type trait versions.")
            );

        static_assert(
            Has<int, Pack<double, float, char, short, int, long>>::Value == true
            && Has_v<int, Pack<double, float, char, short, int, long>> == true
            && Has_native<int, double, float, char, short, int, long>::Value == true
            && Has_native_v<int, double, float, char, short, int, long> == true,

            TestFailureID("Has", 3)
            TestFailureDescription("The given, long pack should contain 'int' and be equal for all type trait versions.")
            );

        static_assert(
            Has<int, Pack<double, float, char, short, int&, long>>::Value == false
            && Has_v<int, Pack<double, float, char, short, int&, long>> == false
            && Has_native<int, double, float, char, short, int&, long>::Value == false
            && Has_native_v<int, double, float, char, short, int&, long> == false,

            TestFailureID("Has", 3)
            TestFailureDescription("The given, long pack should not contain 'int' and should be equal for all type trait versions.")
            );

        // "Find" Test Cases
        // =================

        static_assert(
            Find<void, Pack<>>::Value == NotFound,

            TestFailureID("Find", 1)
            TestFailureDescription("An empty pack should return NotFound for any Find type trait.")
            );

        static_assert(
            Find<int, Pack<char, short, int, long, long long>>::Value == 2
            && Find_v<int, Pack<char, short, int, long, long long>> == 2
            && Find_native<int, char, short, int, long, long long>::Value == 2
            && Find_native_v<int, char, short, int, long, long long> == 2,

            TestFailureID("Find", 2)
            TestFailureDescription("The given pack should have 'int' at position 2 and be equal for all type trait versions.")
            );

        // "Unique" Test Cases
        // ===================

        static_assert(
            Unique<Pack<>>::Value == true,

            TestFailureID("Unique", 1)
            TestFailureDescription("An empty pack should be considered unique.")
            );

        static_assert(
            Unique<Pack<int>>::Value == true,

            TestFailureID("Unique", 2)
            TestFailureDescription("A pack with only one type should be considered unique.")
            );

        static_assert(
            Unique<Pack<int, int>>::Value == false
            && Unique<Pack<int, int, int>>::Value == false
            && Unique<Pack<int, int, int, int>>::Value == false
            && Unique<Pack<int, int, int, int, int>>::Value == false,

            TestFailureID("Unique", 3)
            TestFailureDescription("A pack with any number of duplicate types should not be considered unique.")
            );

        static_assert(
            Unique<Pack<char, short, int, long, long long>>::Value == true
            && Unique_v<Pack<char, short, int, long, long long>> == true
            && Unique_native<char, short, int, long, long long>::Value == true
            && Unique_native_v<char, short, int, long, long long> == true,

            TestFailureID("Unique", 4)
            TestFailureDescription("All type trait versions should agree that the given pack is unique.")
            );

        static_assert(
            Unique<Pack<char, short, int, long, long long, int>>::Value == false
            && Unique_v<Pack<char, short, int, long, long long, int>> == false
            && Unique_native<char, short, int, long, long long, int>::Value == false
            && Unique_native_v<char, short, int, long, long long, int> == false,

            TestFailureID("Unique", 5)
            TestFailureDescription("All type trait versions should agree that the given pack is not unique.")
            );
    }
}

#undef TestFailureID
#undef TestFailureDescription

#endif // PACKHELPER_TEST_CASES_H
