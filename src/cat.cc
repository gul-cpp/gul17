/**
 * \file  cat.cc
 * \brief Implementation of cat() for two or more parameters.
 * \date  Created on August 31, 2018
 *
 * \copyright Copyright 2018-2019 Deutsches Elektronen-Synchrotron (DESY), Hamburg
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
