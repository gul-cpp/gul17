/**
 * \file  case_ascii.cc
 * \brief Implementation of lowercase_ascii(), lowercase_ascii_inplace(),
 *        uppercase_ascii(), and uppercase_ascii_inplace().
 * \date  Created on 28 May 2019
 *
 * \copyright Copyright 2019 Deutsches Elektronen-Synchrotron (DESY), Hamburg
 *
 * Distributed under the Boost Software License, Version 1.0 (see \ref license_boost_1_0
 * or https://www.boost.org/LICENSE_1_0.txt).
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
