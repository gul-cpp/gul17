/**
 * \file  case_ascii.cc
 * \brief Implementation of lowercase_ascii(), lowercase_ascii_inplace(),
 *        uppercase_ascii(), and uppercase_ascii_inplace().
 * \date  Created on 28 May 2019
 *
 * \copyright Copyright 2019 Deutsches Elektronen-Synchrotron (DESY), Hamburg
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

#include "gul17/case_ascii.h"

namespace gul17 {

std::string lowercase_ascii(std::string_view str)
{
    std::string result;
    result.reserve(str.size());

    for (char c : str)
        result.push_back(lowercase_ascii(c));

    return result;
}

std::string &lowercase_ascii_inplace(std::string &str) noexcept
{
    for (char &c : str)
        c = lowercase_ascii(c);

    return str;
}

std::string uppercase_ascii(std::string_view str)
{
    std::string result;
    result.reserve(str.size());

    for (char c : str)
        result.push_back(uppercase_ascii(c));

    return result;
}

std::string &uppercase_ascii_inplace(std::string &str) noexcept
{
    for (char &c : str)
        c = uppercase_ascii(c);

    return str;
}

} // namespace gul17

/* vim:set expandtab softtabstop=4 tabstop=4 shiftwidth=4 textwidth=90 cindent: */
