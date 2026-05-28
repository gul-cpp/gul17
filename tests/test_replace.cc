/**
 * \file  test_replace.cc
 * \date  Created on August 31, 2018
 * \brief Part of test suite for string utility functions in the General Utility Library.
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
