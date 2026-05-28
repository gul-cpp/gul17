/**
 * \file  test_OverloadSet.cc
 * \date  Created on May 31, 2023
 * \brief Unit tests for the OverloadSet class template.
 *
 * \copyright Copyright 2023-2025 Deutsches Elektronen-Synchrotron (DESY), Hamburg
 *
 * Distributed under the Boost Software License, Version 1.0 (see \ref license_boost_1_0
 * or https://www.boost.org/LICENSE_1_0.txt).
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
