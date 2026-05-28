/**
 * \file  escape.h
 * \brief Declaration of escape(), unescape().
 * \date  Created on 31 August 2018
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

#ifndef GUL17_ESCAPE_H_
#define GUL17_ESCAPE_H_

#include <string_view>
#include <string>

#include "gul17/internal.h"

namespace gul17 {

/**
 * \addtogroup escape_h gul17/escape.h
 * \brief Escaping and unescaping special characters in strings.
 * @{
 */

/**
 * Create a new string that looks like an ASCII-only C string literal of the input string.
 * This is achieved by replacing all non-printable and non-ASCII characters with a hex
 * code escape in the form `\x01`.
 *
 * A few special cases are implemented to give more readable representations for very
 * common control characters, and of course backslash and double quotes are escaped as
 * well:
 *
 *     CR   ->  \r
 *     NL   ->  \n
 *     TAB  ->  \t
 *     \    ->  \\
 *     "    ->  \"
 *
 * <h4>Example</h4>
 * \code
 * std::cout << escape("Zwei\tFlüsse\nfließen ins Meer.") << "\n";
 * \endcode
 * Output (assuming that the string literal was in Latin-1 encoding):
 *
 *     Zwei\tFl\xfcsse\nflie\xdfen ins Meer.
 *
 * \note
 * The hexadecimal escape always uses two digits. This is different from the C/C++
 * standard, where it can be an arbitrary number of digits. The standard's way makes it
 * impossible to have any hex digit after a hex escape, e.g. `"\x200"` is invalid and not
 * equal to `" 0"` from the standard's point of view.
 *
 * \param in The input string.
 *
 * \returns a new string that contains only ASCII characters.
 *
 * \since GUL version 1.4 parameter \b in is a std::string_view (was std::string before)
 */
GUL_EXPORT
std::string escape(std::string_view in);

/**
 * Evaluate a string with escaped characters to get the original string back.
 * Does only know the escape sequences used by gul17::escape() and can be
 * used as in inverse function.
 *
 * \param in The string with escape sequences
 *
 * \return A new string where the sequences have been evaluated
 *
 * \since GUL version 1.4 parameter \b in is a std::string_view (was std::string before)
 */
GUL_EXPORT
std::string unescape(std::string_view in);

/// @}

} // namespace gul17

#endif

/* vim:set noexpandtab softtabstop=4 tabstop=4 shiftwidth=4 textwidth=90 cindent: */
