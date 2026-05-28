/**
 * \file  string_util.cc
 * \brief Implementation of string utilities for the General Utility Library.
 * \date  Created on 31 August 2018
 *
 * \copyright Copyright 2018-2026 Deutsches Elektronen-Synchrotron (DESY), Hamburg
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

#include <algorithm>

#include "gul17/string_util.h"

namespace gul17 {

const std::string_view default_whitespace_characters{ " \t\r\n\a\b\f\v" };
const std::array<char, 16> hex_digits{
    { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'} };

std::string null_safe_string(const char* char_ptr)
{
    std::string result;

    if (char_ptr)
        result = char_ptr;

    return result;
}

std::string_view null_safe_string_view(const char* char_ptr)
{
    std::string_view result;

    if (char_ptr)
        result = char_ptr;

    return result;
}

std::string_view null_safe_string_view(const char* char_ptr, std::size_t length)
{
    std::string_view result;

    if (char_ptr)
        result = std::string_view(char_ptr, length);

    return result;
}

std::string repeat(std::string_view str, std::size_t n)
{
    std::string result;

    result.reserve(str.size() * n);
    for (std::size_t i = 0; i != n; ++i)
        result += str;

    return result;
}

std::string safe_string(const char* char_ptr, std::size_t length)
{
    if (char_ptr == nullptr)
        return "";

    auto end_ptr = std::find(char_ptr, char_ptr + length, '\0');

    return std::string(char_ptr, end_ptr);
}

std::string_view safe_string_view(const char* char_ptr, std::size_t length)
{
    if (char_ptr == nullptr)
        return {};

    auto end_ptr = std::find(char_ptr, char_ptr + length, '\0');

    return std::string_view(char_ptr, static_cast<std::size_t>(end_ptr - char_ptr));
}

} // namespace gul17

/* vim:set expandtab softtabstop=4 tabstop=4 shiftwidth=4 textwidth=90 cindent: */
