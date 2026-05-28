/**
 * \file  test_trim.cc
 * \date  Created on September 19, 2018
 * \brief Unit tests for trim(), trim_left(), trim_right(), trim_inplace(),
 *        trim_left_inplace(), and trim_right_inplace().
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

#include "gul17/trim.h"

using gul17::trim;
using gul17::trim_sv;
using gul17::trim_left;
using gul17::trim_left_sv;
using gul17::trim_right;
using gul17::trim_right_sv;

using namespace std::literals;

TEST_CASE("trim() works with various examples", "[trim]")
{
    REQUIRE(trim("  Hello\n World\t\r   ") == "Hello\n World");
    REQUIRE(trim("Hello\t\r   ") == "Hello");
    REQUIRE(trim(" \t\r   Hello") == "Hello");
    REQUIRE(trim("Hello") == "Hello");
    REQUIRE(trim(" \n \t ") == "");
    REQUIRE(trim("") == "");
    REQUIRE(trim("\0Hello\0"s) == "\0Hello\0"s);

    REQUIRE(trim("\0 Hello \0"s, "\0 \t"s) == "Hello"s);
    REQUIRE(trim("Hello World", "Held") == "o Wor");
    REQUIRE(trim(" Hello World ", "") == " Hello World ");
}

TEST_CASE("trim_sv() works with various examples", "[trim]")
{
    REQUIRE(trim_sv("  Hello\n World\t\r   ") == "Hello\n World");
    REQUIRE(trim_sv("Hello\t\r   ") == "Hello");
    REQUIRE(trim_sv(" \t\r   Hello") == "Hello");
    REQUIRE(trim_sv("Hello") == "Hello");
    REQUIRE(trim_sv(" \n \t ") == "");
    REQUIRE(trim_sv("") == "");
    REQUIRE(trim_sv("\0Hello\0"s) == "\0Hello\0"s);

    REQUIRE(trim_sv("\0 Hello \0"s, "\0 \t"s) == "Hello"s);
    REQUIRE(trim_sv("Hello World", "Held") == "o Wor");
    REQUIRE(trim_sv(" Hello World ", "") == " Hello World ");
}

TEST_CASE("trim_left() works with various examples", "[trim]")
{
    REQUIRE(trim_left("  Hello\n World\t\r   ") == "Hello\n World\t\r   ");
    REQUIRE(trim_left("Hello\t\r   ") == "Hello\t\r   ");
    REQUIRE(trim_left(" \t\r   Hello") == "Hello");
    REQUIRE(trim_left("Hello") == "Hello");
    REQUIRE(trim_left(" \n \t ") == "");
    REQUIRE(trim_left("") == "");
    REQUIRE(trim_left("\0Hello\0"s) == "\0Hello\0"s);

    REQUIRE(trim_left("\0 Hello \0"s, "\0 \t"s) == "Hello \0"s);
    REQUIRE(trim_left("Hello World", "Hel") == "o World");
    REQUIRE(trim_left(" Hello World ", "") == " Hello World ");
}

TEST_CASE("trim_left_sv() works with various examples", "[trim]")
{
    REQUIRE(trim_left_sv("  Hello\n World\t\r   ") == "Hello\n World\t\r   ");
    REQUIRE(trim_left_sv("Hello\t\r   ") == "Hello\t\r   ");
    REQUIRE(trim_left_sv(" \t\r   Hello") == "Hello");
    REQUIRE(trim_left_sv("Hello") == "Hello");
    REQUIRE(trim_left_sv(" \n \t ") == "");
    REQUIRE(trim_left_sv("") == "");
    REQUIRE(trim_left_sv("\0Hello\0"s) == "\0Hello\0"s);

    REQUIRE(trim_left_sv("\0 Hello \0"s, "\0 \t"s) == "Hello \0"s);
    REQUIRE(trim_left_sv("Hello World", "Hel") == "o World");
    REQUIRE(trim_left_sv(" Hello World ", "") == " Hello World ");
}

TEST_CASE("trim_right() works with various examples", "[trim]")
{
    REQUIRE(trim_right("  Hello\n World\t\r   ") == "  Hello\n World");
    REQUIRE(trim_right("Hello\t\r   ") == "Hello");
    REQUIRE(trim_right(" \t\r   Hello") == " \t\r   Hello");
    REQUIRE(trim_right("Hello") == "Hello");
    REQUIRE(trim_right(" \n \t ") == "");
    REQUIRE(trim_right("") == "");
    REQUIRE(trim_right("\0Hello\0"s) == "\0Hello\0"s);

    REQUIRE(trim_right("\0 Hello \0"s, "\0 \t"s) == "\0 Hello"s);
    REQUIRE(trim_right("Hello World", "ldr") == "Hello Wo");
    REQUIRE(trim_right(" Hello World ", "") == " Hello World ");
}

TEST_CASE("trim_right_sv() works with various examples", "[trim]")
{
    REQUIRE(trim_right_sv("  Hello\n World\t\r   ") == "  Hello\n World");
    REQUIRE(trim_right_sv("Hello\t\r   ") == "Hello");
    REQUIRE(trim_right_sv(" \t\r   Hello") == " \t\r   Hello");
    REQUIRE(trim_right_sv("Hello") == "Hello");
    REQUIRE(trim_right_sv(" \n \t ") == "");
    REQUIRE(trim_right_sv("") == "");
    REQUIRE(trim_right_sv("\0Hello\0"s) == "\0Hello\0"s);

    REQUIRE(trim_right_sv("\0 Hello \0"s, "\0 \t"s) == "\0 Hello"s);
    REQUIRE(trim_right_sv("Hello World", "ldr") == "Hello Wo");
    REQUIRE(trim_right_sv(" Hello World ", "") == " Hello World ");
}

// vi:ts=4:sw=4:sts=4:et
