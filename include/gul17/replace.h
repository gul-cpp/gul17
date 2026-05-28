/**
 * \file  replace.h
 * \brief Declarations of replace(), replace_inplace().
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

#ifndef GUL17_REPLACE_H_
#define GUL17_REPLACE_H_

#include <string_view>
#include <string>

#include "gul17/internal.h"

namespace gul17 {

/**
 * \addtogroup replace_h gul17/replace.h
 * \brief Replacing all occurrences of a string within another string.
 * @{
 */

/**
 * Replace all occurrences of a string within another string, returning the result
 * as a std::string.
 * Specifically, replace all occurrences of needle within haystack by hammer.
 *
 * \param haystack  The string in which search&replace should take place.
 * \param needle  The string to be searched for. If this is an empty string, a copy
 *                of haystack is returned.
 * \param hammer  The replacement string.
 */
GUL_EXPORT
std::string replace(std::string_view haystack, std::string_view needle, std::string_view hammer);

/**
 * Replace all occurrences of a string within another string in-place.
 * This function replaces all occurrences of needle within haystack by hammer and
 * returns a reference to the modified haystack.
 *
 * \param haystack  The string in which search&replace should take place.
 * \param needle  The string to be searched for. If this is an empty string, haystack is
 *                not changed.
 * \param hammer  The replacement string.
 *
 * \returns the altered haystack.
 */
GUL_EXPORT
std::string& replace_inplace(std::string& haystack, std::string_view needle, std::string_view hammer);

/// @}

} // namespace gul17

#endif

/* vim:set noexpandtab softtabstop=4 tabstop=4 shiftwidth=4 textwidth=90 cindent: */
