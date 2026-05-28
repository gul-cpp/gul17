/**
 * \file    replace.cc
 * \brief   Implementation of replace(), replace_inplace().
 * \date    Created on 31 August 2018
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

#include "gul17/replace.h"

using namespace std::literals::string_literals;

namespace gul17 {

std::string replace(std::string_view haystack, std::string_view needle, std::string_view hammer)
{
    auto result = ""s;
    result.reserve(haystack.length());
    if (needle.empty()) {
        result.assign(haystack.data(), haystack.length());
        return result;
    }

    std::size_t pos = 0;
    std::size_t last_pos = 0;
    while ((pos = haystack.find(needle.data(), pos, needle.length())) != std::string::npos)
    {
        result.append(haystack.data() + last_pos, pos - last_pos);
        result.append(hammer.data(), hammer.length());
        pos += needle.length();
        last_pos = pos;
    }

    result.append(haystack.data() + last_pos, haystack.length() - last_pos);
    result.shrink_to_fit();

    return result;
}

std::string& replace_inplace(std::string& haystack, std::string_view needle, std::string_view hammer)
{
    if (needle.empty())
        return haystack;

    auto pos = haystack.find(needle.data(), 0, needle.length());
    while (pos != std::string::npos) {
        haystack.erase(pos, needle.length());
        haystack.insert(pos, hammer.data(), hammer.length());
        pos = haystack.find(needle.data(), pos + hammer.length(), needle.length());
    }
    return haystack;
}

} // namespace gul17

/* vim:set expandtab softtabstop=4 tabstop=4 shiftwidth=4 textwidth=90 cindent: */
