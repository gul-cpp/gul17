/**
 * \file   test_backports.cc
 * \author \ref contributors
 * \date   Created on August 30, 2018
 * \brief  Test suite for standard library backports in the General Utility Library.
 *
 * \copyright Copyright 2018-2024 Deutsches Elektronen-Synchrotron (DESY), Hamburg
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

#include "gul17/catch.h"
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
