/**
 * \file  trim.cc
 * \date  Created on September 19, 2018
 * \brief Implementation of trim(), trim_left(), trim_right(), trim_sv(), trim_left_sv(),
 *        and trim_right_sv().
 *
 * \copyright Copyright 2018 Deutsches Elektronen-Synchrotron (DESY), Hamburg
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

#include "gul17/trim.h"

namespace gul17 {


std::string trim(std::string_view str, std::string_view ws_chars)
{
    return std::string(trim_sv(str, ws_chars));
}

std::string_view trim_sv(std::string_view str, std::string_view ws_chars)
{
    const auto lpos = str.find_first_not_of(ws_chars);

    if (lpos == std::string_view::npos)
        return "";

    const auto rpos = str.find_last_not_of(ws_chars);

    return str.substr(lpos, rpos - lpos + 1);
}

std::string trim_left(std::string_view str, std::string_view ws_chars)
{
    return std::string(trim_left_sv(str, ws_chars));
}

std::string_view trim_left_sv(std::string_view str, std::string_view ws_chars)
{
    const auto pos = str.find_first_not_of(ws_chars);

    if (pos == std::string_view::npos)
        return "";

    return str.substr(pos);
}

std::string trim_right(std::string_view str, std::string_view ws_chars)
{
    return std::string(trim_right_sv(str, ws_chars));
}

std::string_view trim_right_sv(std::string_view str, std::string_view ws_chars)
{
    const auto pos = str.find_last_not_of(ws_chars);

    if (pos == std::string_view::npos)
        return "";

    return str.substr(0, pos + 1);
}


} // namespace gul17
