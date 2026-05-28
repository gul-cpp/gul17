/**
 * \file  internal.h
 * \brief Definition of macros used internally by GUL.
 * \date  Created on 7 Aug 2019
 *
 * \copyright Copyright 2019-2020 Deutsches Elektronen-Synchrotron (DESY), Hamburg
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this
 * software and associated documentation files (the "Software"), to deal in the Software
 * without restriction, including without limitation the rights to use, copy, modify,
 * merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies
 * or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR
 * THE USE OR OTHER DEALINGS IN THE SOFTWARE.
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
