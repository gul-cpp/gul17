/**
 * \file  internal.h
 * \brief Definition of macros used internally by GUL.
 * \date  Created on 7 Aug 2019
 *
 * \copyright Copyright 2019-2020 Deutsches Elektronen-Synchrotron (DESY), Hamburg
 *
 * Distributed under the Boost Software License, Version 1.0 (see \ref license_boost_1_0
 * or https://www.boost.org/LICENSE_1_0.txt).
 */

#ifndef GUL17_INTERNAL_H_
#define GUL17_INTERNAL_H_

// For old Visual C++ compilers, including <ciso646> enables the alternative operator
// representations "and", "or", and "not".
#if defined(_MSC_VER)
#   include <ciso646>
#endif

// The GUL_EXPORT macro is used to export certain symbols to the generated library.
// How the symbols have to me marked is different between compilers.
#if defined(_MSC_VER)
#   if defined(GUL_COMPILING_SHARED_LIB)
#       define GUL_EXPORT __declspec(dllexport)
#   elif defined(GUL_USING_STATIC_LIB_OR_OBJECTS)
#       define GUL_EXPORT
#   else
#       define GUL_EXPORT __declspec(dllimport)
#   endif
#elif defined(__GNUC__)
#   define GUL_EXPORT __attribute__ ((visibility ("default")))
#else
#   define GUL_EXPORT
#endif

#endif

// vi:ts=4:sw=4:et:sts=4
