/**
 * \file   test_replace.cc
 * \author \ref contributors
 * \date   Created on August 31, 2018
 * \brief  Part of test suite for string utility functions in the General Utility Library.
 *
 * \copyright Copyright 2018-2025 Deutsches Elektronen-Synchrotron (DESY), Hamburg
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

#include <catch2/catch_test_macros.hpp>

#include "gul17/replace.h"

using namespace std::literals;
using namespace gul17;

TEST_CASE("Replace parts of a string in-place", "[replace]")
{
    const std::string input{ "foo bar baz" };
    std::string foo = input;

    SECTION("replacing foo by foo") {
        REQUIRE(replace_inplace(foo, "foo", "foo") == input); // replace the one with one
    }
    SECTION("replacing world by foo") {
        REQUIRE(replace_inplace(foo, "world", "foo") == input); // no match
    }
    SECTION("replacing foo by world") {
        REQUIRE(replace_inplace(foo, "foo", "world") == "world bar baz");
    }
    SECTION("replacing f by m") {
        REQUIRE(replace_inplace(foo, "f", "m") == "moo bar baz"s );
    }
    SECTION("replacing spaces by tabs") {
        REQUIRE(replace_inplace(foo, " ", "\t") == "foo\tbar\tbaz"s );
    }
    SECTION("empty needle does nothing") {
	    REQUIRE(replace_inplace(foo, "", "\t") == input);
	}

	foo = "";
    REQUIRE(replace_inplace(foo, " ", "\t") == ""s);
}

TEST_CASE("Replace parts of a string using replace()", "[replace]")
{
    REQUIRE(replace("hello lovely helpful world", "he", "KOU") == "KOUllo lovely KOUlpful world");
    REQUIRE(replace("hello lovely helpful world", "he", "A") == "Allo lovely Alpful world");
    REQUIRE(replace("hello lovely helpful world", "", "AGA") == "hello lovely helpful world");
    REQUIRE(replace("hello lovely helpful world", "lo", "y") == "hely yvely helpful world");
    REQUIRE(replace("hello lovely helpful world", "l", "") == "heo ovey hepfu word");
}

// vi:ts=4:sw=4:sts=4:et
