/**
 * \file  test_case_ascii.cc
 * \date  Created on May 28, 2019
 * \brief Test suite for lowercase_ascii(), lowercase_ascii_inplace(), uppercase_ascii(),
 *        and uppercase_ascii_inplace().
 *
 * \copyright Copyright 2019-2025 Deutsches Elektronen-Synchrotron (DESY), Hamburg
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

#include <catch2/catch_test_macros.hpp>

#include "gul17/case_ascii.h"

using namespace std::literals;
using namespace gul17;

TEST_CASE("lowercase_ascii()", "[case_ascii]")
{
    const std::string ustr = "ABCDEFGHIJKLMNOPQRSTUVWXYZ-+öä";
    const std::string lstr = "abcdefghijklmnopqrstuvwxyz-+öä";

    REQUIRE(ustr.length() == lstr.length());

    REQUIRE(lowercase_ascii(ustr) == lstr);

    for (auto i = 0u; i < ustr.length(); i++)
        REQUIRE(lowercase_ascii(ustr[i]) == lstr[i]);
}

TEST_CASE("lowercase_ascii(char) is constexpr", "[case_ascii]")
{
    constexpr char c = lowercase_ascii('B');
    REQUIRE(c == 'b');
}

TEST_CASE("lowercase_ascii_inplace()", "[case_ascii]")
{
    std::string ustr = "ABCDEFGHIJKLMNOPQRSTUVWXYZ-+öä";
    const std::string lstr = "abcdefghijklmnopqrstuvwxyz-+öä";

    REQUIRE(ustr.length() == lstr.length());

    const auto adr_old = &ustr;

    auto &ref = lowercase_ascii_inplace(ustr);

    REQUIRE(ustr == lstr);

    auto adr_returned = &ref;

    REQUIRE(adr_returned == adr_old);
}

TEST_CASE("uppercase_ascii()", "[case_ascii]")
{
    const std::string ustr = "ABCDEFGHIJKLMNOPQRSTUVWXYZ-+öä";
    const std::string lstr = "abcdefghijklmnopqrstuvwxyz-+öä";

    REQUIRE(ustr.length() == lstr.length());

    REQUIRE(uppercase_ascii(lstr) == ustr);

    for (auto i = 0u; i < ustr.length(); i++)
        REQUIRE(uppercase_ascii(lstr[i]) == ustr[i]);
}

TEST_CASE("uppercase_ascii(char) is constexpr", "[case_ascii]")
{
    constexpr char c = uppercase_ascii('b');
    REQUIRE(c == 'B');
}

TEST_CASE("uppercase_ascii_inplace()", "[case_ascii]")
{
    const std::string ustr = "ABCDEFGHIJKLMNOPQRSTUVWXYZ-+öä";
    std::string lstr = "abcdefghijklmnopqrstuvwxyz-+öä";

    REQUIRE(ustr.length() == lstr.length());

    const auto adr_old = &lstr;

    auto &ref = uppercase_ascii_inplace(lstr);

    REQUIRE(lstr == ustr);

    auto adr_returned = &ref;

    REQUIRE(adr_returned == adr_old);
}

// vi:ts=4:sw=4:sts=4:et
