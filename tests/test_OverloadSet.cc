/**
 * \file  test_OverloadSet.cc
 * \date  Created on May 31, 2023
 * \brief Unit tests for the OverloadSet class template.
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

#include <string>
#include <variant>

#include <catch2/catch_test_macros.hpp>

#include "gul17/OverloadSet.h"

TEST_CASE("OverloadSet: Constructor", "[variant]")
{
    auto v = std::variant<float, double, long, std::string>{ };

    auto get_type_name = gul17::OverloadSet{
        [](auto&&) -> std::string { return "SOMETHING ELSE"; },
        [](float) -> std::string { return "float"; },
        [](double) -> std::string { return "double"; },
        [](long) -> std::string { return "long"; }
    };

    v = 1.5f;
    REQUIRE(std::visit(get_type_name, v) == "float");

    v = 2.4;
    REQUIRE(std::visit(get_type_name, v) == "double");

    v = 42L;
    REQUIRE(std::visit(get_type_name, v) == "long");

    v = "Test";
    REQUIRE(std::visit(get_type_name, v) == "SOMETHING ELSE");
}
