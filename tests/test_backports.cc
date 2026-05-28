/**
 * \file  test_backports.cc
 * \date  Created on August 30, 2018
 * \brief Test suite for standard library backports in the General Utility Library.
 *
 * \copyright Copyright 2018-2025 Deutsches Elektronen-Synchrotron (DESY), Hamburg
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

#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>

#include "gul17/date.h"
#include "gul17/span.h"

using namespace std::literals;

TEMPLATE_TEST_CASE("span", "[span]", signed char, unsigned char, short, unsigned short,
                   int, unsigned int, long, unsigned long, long long, unsigned long long)
{
    TestType arr[5] { 1, 2, 3, 4, 5 };

    gul17::span<TestType> s;
    REQUIRE(s.empty());

    s = arr;
    REQUIRE(s.size() == 5u);

    for (auto i = 0u; i < s.size(); i++)
        REQUIRE(arr[i] == s[i]);
}

TEST_CASE("date", "[date]")
{
    using namespace gul17::date;

    constexpr year_month_day ymd = 2015_y/March/22;

    REQUIRE(format("%Y-%m-%d", ymd) == "2015-03-22");

    constexpr auto sd = sys_days{ ymd };
    REQUIRE(format("%Y-%m-%dT%H:%M:%OS", sd) == "2015-03-22T00:00:00");
}

// vi:ts=4:sw=4:sts=4:et
