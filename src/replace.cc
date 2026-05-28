/**
 * \file    replace.cc
 * \brief   Implementation of replace(), replace_inplace().
 * \date    Created on 31 August 2018
 *
 * \copyright Copyright 2018 Deutsches Elektronen-Synchrotron (DESY), Hamburg
 *
 * Distributed under the Boost Software License, Version 1.0 (see \ref license_boost_1_0
 * or https://www.boost.org/LICENSE_1_0.txt).
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
