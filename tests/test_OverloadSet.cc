/**
 * \file   test_variant.cc
 * \author \ref contributors
 * \date   Created on May 31, 2023
 * \brief  Unit tests for the OverloadSet class template.
 *
 * \copyright Copyright 2023-2025 Deutsches Elektronen-Synchrotron (DESY), Hamburg
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
