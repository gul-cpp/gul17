/**
 * \file    string_util.cc
 * \brief   Implementation of string utilities for the General Utility Library.
 * \authors \ref contributors
 * \date    Created on 31 August 2018
 *
 * \copyright Copyright 2018-2025 Deutsches Elektronen-Synchrotron (DESY), Hamburg
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 2.1 of the license, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
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

    return std::string_view(char_ptr, end_ptr - char_ptr);
}

} // namespace gul17

/* vim:set expandtab softtabstop=4 tabstop=4 shiftwidth=4 textwidth=90 cindent: */
