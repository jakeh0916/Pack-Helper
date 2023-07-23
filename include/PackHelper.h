#ifndef PACKHELPER_PACKHELPER_H
#define PACKHELPER_PACKHELPER_H

// ===============================
// | PackHelper Library Header   |
// | Copyright 2023 Jake Huseman |
// ===============================

// This file includes the core PackHelper headers, checks the C++ version,
// and allows test cases to be easily compiled in a single translation unit.
//
// Control Macros:
//
// PACKHELPER__RUN_TESTS
//       - Define this token to compile/run all PackHelper test cases.
// PACKHELPER__QUIET
//       - Define this token to quiet any compiler errors/warnings from PackHelper.

#include "core/Pack.h"
#include "core/Traits.h"

#ifndef PACKHELPER__QUIET
#   define C_PLUS_PLUS_20_VER 202002L
#   if ((defined(_MSVC_LANG) && _MSVC_LANG < C_PLUS_PLUS_20_VER))
#       error C++ 20 is currently required to use the PackHelper library.
#   elseif (__cplusplus < C_PLUS_PLUS_20_VER)
#       error C++ 20 is currently required to use the PackHelper library.
#   endif
#endif

#ifdef PACKHELPER__RUN_TESTS
#   include "testcases/TestCases.h"
#endif

#endif // PACKHELPER_PACKHELPER_H
