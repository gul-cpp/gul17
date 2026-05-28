/**
 * \file  test_expected.cc
 * \date  Created on March 20, 2023
 * \brief Unit tests for the expected class template.
 *
 * \copyright Copyright 2023-2025 Deutsches Elektronen-Synchrotron (DESY), Hamburg
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

#include <memory>
#include <optional>
#include <string>

#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>

#include "gul17/expected.h"

using namespace std::literals;

// Note: As this is just a backport of std::expected, we do not include a substantial
// test suite. The tests are just to make sure that the backport works roughly as
// expected (no pun intended).

namespace {

gul17::expected<int, std::string> unexpected_if_negative(int value)
{
    if (value < 0)
        return gul17::unexpected<std::string>("error");

    return value;
}

} // namespace

TEMPLATE_TEST_CASE("expected: Default constructor", "[expected]", int, std::string,
    std::unique_ptr<int>)
{
    gul17::expected<TestType, std::string> ex;
    REQUIRE(ex.value() == TestType{});
}

TEST_CASE("expected: and_then()", "[expected]")
{
    auto ex = unexpected_if_negative(42).and_then(unexpected_if_negative);
    REQUIRE(ex == 42);

    auto ex2 = unexpected_if_negative(-42).and_then(unexpected_if_negative);
    REQUIRE(ex2.has_value() == false);
}

TEST_CASE("expected: has_value()", "[expected]")
{
    gul17::expected<int, std::string> ex;

    REQUIRE(ex.has_value() == true);
    REQUIRE(*ex == 0);

    ex = gul17::unexpected<std::string>("error");
    REQUIRE(ex.has_value() == false);
    REQUIRE(ex.error() == "error");

    ex = 42;
    REQUIRE(ex.has_value() == true);
    REQUIRE(*ex == 42);
}

TEST_CASE("value()", "[expected]")
{
    gul17::expected<int, std::string> ex{ 42 };

    REQUIRE(ex.value() == 42);

    ex = gul17::unexpected<std::string>("error");
    REQUIRE_THROWS_AS(ex.value(), gul17::bad_expected_access<void>); // base class
    REQUIRE_THROWS_AS(ex.value(), gul17::bad_expected_access<std::string>); // derived class
}
