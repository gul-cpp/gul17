/**
 * \file   test_bit_manip.cc
 * \author \ref contributors
 * \date   Created on 17 Oct 2019
 * \brief  Unit tests for bit_set(), bit_reset(), bit_flip(), bit_test().
 *
 * \copyright Copyright 2019-2025 Deutsches Elektronen-Synchrotron (DESY), Hamburg
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

#include <type_traits>

#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>

#include "gul17/bit_manip.h"

TEMPLATE_TEST_CASE("All bit_*() functions", "[bit_manip]",
        char, unsigned char, signed char, short, unsigned short, int,
        unsigned int, long, unsigned long, long long, unsigned long long)
{
    auto x = gul17::bit_set<TestType>(2); // 0x04
    REQUIRE(x == 0x04);
    REQUIRE(std::is_same<decltype(x), TestType>::value == true);

    auto y = gul17::bit_set(x, 4); // 0x10
    REQUIRE(y == 0x14);
    REQUIRE(std::is_same<decltype(y), TestType>::value == true);

    auto z = gul17::bit_flip(y, 0); // 0x01
    REQUIRE(z == 0x15);
    REQUIRE(std::is_same<decltype(z), TestType>::value == true);

    z = gul17::bit_flip(z, 2); // 0x04
    REQUIRE(z == 0x11);

    auto a = gul17::bit_reset(z, 4); // 0x10
    REQUIRE(a == 0x01);
    REQUIRE(std::is_same<decltype(a), TestType>::value == true);

    a = gul17::bit_reset(a, 3); // 0x08
    REQUIRE(a == 0x01);

    auto b = gul17::bit_test(a, 0); // 0x01
    REQUIRE(b == true);
    REQUIRE(std::is_same<decltype(b), bool>::value == true);

    b = gul17::bit_test(a, 1); // 0x02
    REQUIRE(b == false);
}

TEMPLATE_TEST_CASE("Access MSB", "[bit_manip]",
        char, unsigned char, signed char, short, unsigned short, int,
        unsigned int, long, unsigned long, long long, unsigned long long)
{
    auto const msb = sizeof(TestType) * 8 - 1;

    auto x = gul17::bit_set<TestType>(msb);

    if (std::is_signed<TestType>::value)
        REQUIRE(x < 0);
    else
        REQUIRE(x > 0);

    REQUIRE(gul17::bit_test(x, msb) == true);
    REQUIRE(gul17::bit_test(x, msb - 1) == false);
}

TEST_CASE("is_little_endian(), is_big_endian()", "[bit_manip]")
{
    REQUIRE(gul17::is_little_endian() != gul17::is_big_endian());

    const std::uint32_t val = 0x01'02'03'04;
    const char* chars = reinterpret_cast<const char*>(&val);

    if (gul17::is_little_endian())
    {
        REQUIRE(chars[0] == 4);
        REQUIRE(chars[1] == 3);
        REQUIRE(chars[2] == 2);
        REQUIRE(chars[3] == 1);
    }
    else if (gul17::is_big_endian())
    {
        REQUIRE(chars[0] == 1);
        REQUIRE(chars[1] == 2);
        REQUIRE(chars[2] == 3);
        REQUIRE(chars[3] == 4);
    }
}

// vi:ts=4:sw=4:sts=4:et
