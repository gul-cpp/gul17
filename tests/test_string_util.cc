/**
 * \file   test_string_util.cc
 * \author \ref contributors
 * \date   Created on August 11, 2021
 * \brief  Unit tests for string utility functions.
 *
 * \copyright Copyright 2021-2025 Deutsches Elektronen-Synchrotron (DESY), Hamburg
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

#include <array>
#include <iomanip>
#include <sstream>
#include <vector>

#include <catch2/catch_test_macros.hpp>

#include "gul17/SmallVector.h"
#include "gul17/string_util.h"

using gul17::hex_string;
using gul17::repeat;

using namespace std::literals;

TEST_CASE("hex_digits", "[string_util]")
{
    REQUIRE(gul17::hex_digits.size() == 16);
}

TEST_CASE("hex_string(Integer)", "[string_util]")
{
    std::stringstream ss;

    for (int i = 0; i != 256; ++i)
    {
        auto val = static_cast<unsigned char>(i);
        CAPTURE(val);
        ss.str("");
        ss << std::setfill('0') << std::setw(2) << std::hex << int(val);

        REQUIRE(ss.str() == hex_string(val));
    }

    REQUIRE(hex_string(static_cast<uint16_t>(15)) == "000f");
    REQUIRE(hex_string(static_cast<uint16_t>(16)) == "0010");
    REQUIRE(hex_string(static_cast<int16_t>(15)) == "000f");
    REQUIRE(hex_string(static_cast<int16_t>(16)) == "0010");
    REQUIRE(hex_string(static_cast<uint32_t>(65535)) == "0000ffff");
    REQUIRE(hex_string(static_cast<uint32_t>(65536)) == "00010000");
    REQUIRE(hex_string(static_cast<int32_t>(-1)) == "ffffffff");
    REQUIRE(hex_string(static_cast<int64_t>(-1)) == "ffffffffffffffff");
    REQUIRE(hex_string(static_cast<int64_t>(0xdeadbeef00000000LL)) == "deadbeef00000000");
}

TEST_CASE("hex_string(Iterator, Iterator, std::string_view)", "[string_util]")
{
    std::array<unsigned char, 4> uc{ { 0, 15, 16, 255 } };
    REQUIRE(hex_string(uc.begin(), uc.begin()) == "");
    REQUIRE(hex_string(uc.begin(), uc.begin(), "-") == "");
    REQUIRE(hex_string(uc.begin(), uc.end()) == "000f10ff");
    REQUIRE(hex_string(uc.begin(), uc.end(), "-") == "00-0f-10-ff");

    uint16_t us[] = { 256, 255 };
    REQUIRE(hex_string(std::begin(us), std::begin(us)) == "");
    REQUIRE(hex_string(std::begin(us), std::begin(us), "-") == "");
    REQUIRE(hex_string(std::begin(us), std::end(us)) == "010000ff");
    REQUIRE(hex_string(std::begin(us), std::end(us), " * ") == "0100 * 00ff");

    int64_t sll[] = { 256, -1, 0 };
    REQUIRE(hex_string(std::begin(sll), std::begin(sll)) == "");
    REQUIRE(hex_string(std::begin(sll), std::begin(sll), "-") == "");
    REQUIRE(hex_string(std::begin(sll), std::end(sll)) ==
            "0000000000000100ffffffffffffffff0000000000000000");
    REQUIRE(hex_string(std::begin(sll), std::end(sll), " ") ==
            "0000000000000100 ffffffffffffffff 0000000000000000");
}

TEST_CASE("hex_string(Array, std::string_view)", "[string_util]")
{
    uint16_t us[] = { 256, 255 };
    REQUIRE(hex_string(us) == "010000ff");
    REQUIRE(hex_string(us, " - ") == "0100 - 00ff");

    int64_t sll[] = { 256, -1, 0 };
    REQUIRE(hex_string(sll) == "0000000000000100ffffffffffffffff0000000000000000");
    REQUIRE(hex_string(sll, " ") == "0000000000000100 ffffffffffffffff 0000000000000000");
}

TEST_CASE("hex_string(Container, std::string_view)", "[string_util]")
{
    std::array<unsigned char, 4> uc{ { 0, 15, 16, 255 } };
    REQUIRE(hex_string(uc) == "000f10ff");
    REQUIRE(hex_string(uc, " ") == "00 0f 10 ff");

    std::vector<uint16_t> us = { 256, 255 };
    REQUIRE(hex_string(us) == "010000ff");
    REQUIRE(hex_string(us, "-") == "0100-00ff");

    gul17::SmallVector<int64_t, 3> sll = { 256, -1, 0 };
    REQUIRE(hex_string(sll) == "0000000000000100ffffffffffffffff0000000000000000");
    REQUIRE(hex_string(sll, "/") == "0000000000000100/ffffffffffffffff/0000000000000000");
}

TEST_CASE("null_safe_string(const char*)", "[string_util]")
{
    using gul17::null_safe_string;

    REQUIRE(null_safe_string(nullptr) == "");
    REQUIRE(null_safe_string("") == "");
    REQUIRE(null_safe_string("hello") == "hello");
    REQUIRE(null_safe_string("hi\0there") == "hi");
}

TEST_CASE("null_safe_string_view(const char*)", "[string_util]")
{
    using gul17::null_safe_string_view;

    REQUIRE(null_safe_string_view(nullptr) == std::string_view{});
    REQUIRE(null_safe_string_view("") == ""sv);
    REQUIRE(null_safe_string_view("hello") == "hello"sv);
    REQUIRE(null_safe_string_view("hi\0there") == "hi"sv);
}

TEST_CASE("repeat()", "[string_util]")
{
    REQUIRE(repeat("du", 3) == "dududu");
    REQUIRE(repeat("du", 0) == "");
    REQUIRE(repeat("", 3) == "");
    REQUIRE(repeat("", 0) == "");
    REQUIRE(repeat("\0\0"s, 2) == "\0\0\0\0"s);
}

TEST_CASE("safe_string(const char*, size_t)", "[string_util]")
{
    using gul17::safe_string;

    REQUIRE(safe_string(nullptr, 0) == "");
    REQUIRE(safe_string(nullptr, 10) == "");
    REQUIRE(safe_string("hello", 0) == "");
    REQUIRE(safe_string("hello", 10) == "hello");
    REQUIRE(safe_string("hello\0", 6) == "hello");
    REQUIRE(safe_string("hello\0world", 11) == "hello");
}

TEST_CASE("safe_string_view(const char*, size_t)", "[string_util]")
{
    using gul17::safe_string_view;

    REQUIRE(safe_string_view(nullptr, 10) == std::string_view{});
    REQUIRE(safe_string_view("", 0) == ""sv);
    REQUIRE(safe_string_view("", 1) == ""sv);
    REQUIRE(safe_string_view("ABC", 0) == ""sv);
    REQUIRE(safe_string_view("ABC", 2) == "AB"sv);
    REQUIRE(safe_string_view("ABC", 3) == "ABC"sv);
    REQUIRE(safe_string_view("ABC", 4) == "ABC"sv);
    REQUIRE(safe_string_view("AB\0CD", 5) == "AB"sv);
}
