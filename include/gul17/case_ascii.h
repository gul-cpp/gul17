/**
 * \file  case_ascii.h
 * \brief Declarations of lowercase_ascii(), lowercase_ascii_inplace(), uppercase_ascii(),
 *        and uppercase_ascii_inplace().
 * \date  Created on 28 May 2019
 *
 * \copyright Copyright 2019-2024 Deutsches Elektronen-Synchrotron (DESY), Hamburg
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

#ifndef GUL17_CASE_ASCII_H_
#define GUL17_CASE_ASCII_H_

#include <string_view>
#include <string>

#include "gul17/internal.h"

namespace gul17 {

/**
 * \addtogroup case_ascii_h gul17/case_ascii.h
 * \brief Converting ASCII characters to lowercase or uppercase.
 * @{
 */

/**
 * Return the ASCII lowercase equivalent of the given character (or the unchanged
 * character, if it is not an ASCII letter).
 * \param c  The original character.
 * \since GUL version 1.2
 * \see lowercase_ascii(std::string_view), lowercase_ascii_inplace(std::string &),
 *      uppercase_ascii(char)
 */
constexpr char lowercase_ascii(char c) noexcept
{
    if (c >= 'A' && c <= 'Z')
        c = static_cast<char>(c + ('a' - 'A'));
    return c;
}

/**
 * Return a copy of the given string in which all ASCII characters are replaced by their
 * lowercase equivalents.
 * \param str  The original string.
 * \since GUL version 1.2
 * \see lowercase_ascii(char), lowercase_ascii_inplace(std::string &),
 *      uppercase_ascii(std::string_view)
 */
GUL_EXPORT
std::string lowercase_ascii(std::string_view str);

/**
 * Replace all ASCII characters in a string by their lowercase equivalents.
 * This function modifies the original string and returns a reference to it (which may
 * be helpful for chaining function calls).
 * \param str  The string to be modified.
 * \returns a reference to the string argument.
 * \since GUL version 1.2
 * \see lowercase_ascii(char), lowercase_ascii(std::string_view),
 *      uppercase_ascii_inplace(std::string &)
 */
GUL_EXPORT
std::string& lowercase_ascii_inplace(std::string& str) noexcept;

/**
 * Return the ASCII uppercase equivalent of the given character (or the unchanged
 * character, if it is not an ASCII letter).
 * \param c  The original character.
 * \since GUL version 1.2
 * \see uppercase_ascii(std::string_view), uppercase_ascii_inplace(std::string &),
 *      lowercase_ascii(char)
 */
constexpr char uppercase_ascii(char c) noexcept
{
    if (c >= 'a' && c <= 'z')
        c = static_cast<char>(c - ('a' - 'A'));
    return c;
}

/**
 * Return a copy of the given string in which all ASCII characters are replaced by their
 * uppercase equivalents.
 * \param str  The original string.
 * \since GUL version 1.2
 * \see uppercase_ascii(char), uppercase_ascii_inplace(std::string &),
 *      lowercase_ascii(std::string_view)
 */
GUL_EXPORT
std::string uppercase_ascii(std::string_view str);

/**
 * Replace all ASCII characters in a string by their uppercase equivalents.
 * This function modifies the original string and returns a reference to it (which may
 * be helpful for chaining function calls).
 * \param str  The string to be modified.
 * \returns a reference to the string argument.
 * \since GUL version 1.2
 * \see uppercase_ascii(char), uppercase_ascii(std::string_view),
 *      lowercase_ascii_inplace(std::string &)
 */
GUL_EXPORT
std::string& uppercase_ascii_inplace(std::string& str) noexcept;

/// @}

} // namespace gul17

#endif

/* vim:set expandtab softtabstop=4 tabstop=4 shiftwidth=4 textwidth=90 cindent: */
