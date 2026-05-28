/**
 * \file  trim.cc
 * \date  Created on September 19, 2018
 * \brief Implementation of trim(), trim_left(), trim_right(), trim_sv(), trim_left_sv(),
 *        and trim_right_sv().
 *
 * \copyright Copyright 2018 Deutsches Elektronen-Synchrotron (DESY), Hamburg
 *
 * Distributed under the Boost Software License, Version 1.0 (see \ref license_boost_1_0
 * or https://www.boost.org/LICENSE_1_0.txt).
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
