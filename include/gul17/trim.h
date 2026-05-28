/**
 * \file  trim.h
 * \date  Created on September 19, 2018
 * \brief Declarations of trim(), trim_left(), trim_right(), trim_sv(), trim_left_sv(),
 *        and trim_right_sv().
 *
 * \copyright Copyright 2018-2024 Deutsches Elektronen-Synchrotron (DESY), Hamburg
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

#ifndef GUL17_TRIM_H_
#define GUL17_TRIM_H_

#include <string_view>

#include "gul17/internal.h"
#include "gul17/string_util.h"

namespace gul17 {

/**
 * \addtogroup trim_h gul17/trim.h
 * \brief Trimming whitespace from strings.
 * @{
 */

/**
 * Trim leading and trailing whitespace (or a custom set of characters) from a string,
 * returning a new std::string.
 * Which characters are removed can be customized via the ws_chars parameter.
 *
 * \snippet trim.cc Using trim() with default and custom whitespace
 *
 * \param str   The string that should be trimmed.
 * \param ws_chars  A string containing all the characters that should be treated as
 *                  whitespace (i.e. that are trimmed). If this is empty, no characters
 *                  are trimmed.
 * \returns a trimmed copy of the input string.
 *
 * \see trim_sv() returns a std::string_view instead of a copied string,<br>
 *      trim_left() and trim_right() %trim only one side of the string,<br>
 *      trim_left_sv() and trim_right_sv() %trim only one side and return a std::string_view.
 */
GUL_EXPORT
std::string trim(std::string_view str, std::string_view ws_chars = default_whitespace_characters);

/**
 * Trim leading and trailing whitespace (or a custom set of characters) from a string,
 * returning a view into the original string.
 * Which characters are removed can be customized via the ws_chars parameter.
 *
 * \snippet trim.cc Using trim_sv() on a mutable string
 *
 * \param str   The string that should be trimmed.
 * \param ws_chars  A string containing all the characters that should be treated as
 *                  whitespace (i.e. that are trimmed). If this is empty, no characters
 *                  are trimmed.
 * \returns a trimmed std::string_view that points into the input string.
 *
 * \see trim() returns a copied string instead of a std::string_view,<br>
 *      trim_left_sv() and trim_right_sv() %trim only one side of the string,<br>
 *      trim_left() and trim_right() %trim only one side and return a copied string.<br>
 */
GUL_EXPORT
std::string_view trim_sv(std::string_view str, std::string_view ws_chars = default_whitespace_characters);

/**
 * Trim leading whitespace (or a custom set of characters) from a string, returning a new
 * std::string.
 * Which characters are removed can be customized via the ws_chars parameter.
 *
 * \snippet trim.cc Using trim_left() and trim_right() with default and custom whitespace
 *
 * \param str   The string from which leading characters should be trimmed.
 * \param ws_chars  A string containing all the characters that should be treated as
 *                  whitespace (i.e. that are trimmed). If this is empty, no characters
 *                  are trimmed.
 * \returns a trimmed copy of the input string.
 *
 * \see trim_left_sv() returns a std::string_view instead of a copied string,<br>
 *      trim_right() and trim_right_sv() %trim the other side of the string,<br>
 *      trim() and trim_sv() %trim both sides of the string.
 */
GUL_EXPORT
std::string trim_left(std::string_view str, std::string_view ws_chars = default_whitespace_characters);

/**
 * Trim leading whitespace (or a custom set of characters) from a string, returning a view
 * into the original string.
 * Which characters are removed can be customized via the ws_chars parameter.
 *
 * \snippet trim.cc Using trim_left_sv() and trim_right_sv() with default and custom whitespace
 *
 * \param str   The string from which leading characters should be trimmed.
 * \param ws_chars  A string containing all the characters that should be treated as
 *                  whitespace (i.e. that are trimmed). If this is empty, no characters
 *                  are trimmed.
 * \returns a trimmed std::string_view that points into the input string.
 *
 * \see trim_left() returns a copied string instead of a std::string_view,<br>
 *      trim_right() and trim_right_sv() %trim the other side of the string,<br>
 *      trim() and trim_sv() %trim both sides of the string.
 */
GUL_EXPORT
std::string_view trim_left_sv(std::string_view str, std::string_view ws_chars = default_whitespace_characters);

/**
 * Trim trailing whitespace (or a custom set of characters) from a string, returning a new
 * std::string.
 * Which characters are removed can be customized via the ws_chars parameter.
 *
 * \snippet trim.cc Using trim_left() and trim_right() with default and custom whitespace
 *
 * \param str   The string from which trailing characters should be trimmed.
 * \param ws_chars  A string containing all the characters that should be treated as
 *                  whitespace (i.e. that are trimmed). If this is empty, no characters
 *                  are trimmed.
 * \returns a trimmed copy of the input string.
 *
 * \see trim_right_sv() returns a std::string_view instead of a copied string,<br>
 *      trim_left() and trim_left_sv() %trim the other side of the string,<br>
 *      trim() and trim_sv() %trim both sides of the string.
 */
GUL_EXPORT
std::string trim_right(std::string_view str, std::string_view ws_chars = default_whitespace_characters);

/**
 * Trim trailing whitespace (or a custom set of characters) from a string, returning a
 * view into the original string.
 * Which characters are removed can be customized via the ws_chars parameter.
 *
 * \snippet trim.cc Using trim_left_sv() and trim_right_sv() with default and custom whitespace
 *
 * \param str   The string from which trailing characters should be trimmed.
 * \param ws_chars  A string containing all the characters that should be treated as
 *                  whitespace (i.e. that are trimmed). If this is empty, no characters
 *                  are trimmed.
 * \returns a trimmed std::string_view that points into the input string.
 *
 * \see trim_right() returns a copied string instead of a std::string_view,<br>
 *      trim_left() and trim_left_sv() %trim the other side of the string,<br>
 *      trim() and trim_sv() %trim both sides of the string.
 */
GUL_EXPORT
std::string_view trim_right_sv(std::string_view str, std::string_view ws_chars = default_whitespace_characters);

// TODO: Add trim_*_inplace

/// @}

} // namespace gul17


/**
 * \example trim.cc
 * A simple example on how to use the trim() family of functions for stripping characters
 * from one or both sides of a string.
 */

#endif
