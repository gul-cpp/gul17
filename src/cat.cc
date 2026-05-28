/**
 * \file  cat.cc
 * \brief Implementation of cat() for two or more parameters.
 * \date  Created on August 31, 2018
 *
 * \copyright Copyright 2018-2019 Deutsches Elektronen-Synchrotron (DESY), Hamburg
 *
 * Distributed under the Boost Software License, Version 1.0 (see \ref license_boost_1_0
 * or https://www.boost.org/LICENSE_1_0.txt).
 */

#include "gul17/cat.h"

namespace gul17 {


std::string cat(const ConvertingStringView& s1, const ConvertingStringView& s2)
{
    std::string str;
    str.reserve(s1.length() + s2.length());
    str.append(s1.data(), s1.length());
    str.append(s2.data(), s2.length());
    return str;
}

std::string cat(const ConvertingStringView& s1, const ConvertingStringView& s2, const ConvertingStringView& s3)
{
    std::string str;
    str.reserve(s1.length() + s2.length() + s3.length());
    str.append(s1.data(), s1.length());
    str.append(s2.data(), s2.length());
    str.append(s3.data(), s3.length());
    return str;
}

std::string cat(std::initializer_list<ConvertingStringView> pieces)
{
    std::string str;

    std::size_t len = 0;
    for (const ConvertingStringView &piece : pieces)
        len += piece.size();

    str.reserve(len);

    for (const ConvertingStringView &piece : pieces)
        str.append(piece.data(), piece.size());

    return str;
}

} // namespace gul17
