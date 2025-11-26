/**
 * \file   test_JsonDataProcessor.cc
 * \author Jan Behrens
 * \date   Created on November 19, 2025
 * \brief  Test suite for the JsonDataProcessor class.
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

#include "gul17/data_processors.h"

#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#if 0 // Temporarily disable all tests to focus on test_func()

using gul17::DataTree;
using gul17::from_json_string;
using gul17::to_json_string;

TEST_CASE("JsonDataProcessor: JSON parsing", "[JsonDataProcessor]")
{
    auto tree = from_json_string(
        R"({"key1": "value1", "key2": 42, "key3": [1, 2, 3], "key4": {"nestedKey": 3.1415}, "key5": null})");

    REQUIRE(tree["key1"].is_string());
    REQUIRE(tree["key1"].as<std::string>() == "value1");

    REQUIRE(tree["key2"].is_number());
    REQUIRE(tree["key2"].as<int>() == 42);

    REQUIRE(tree["key3"].is_array());
    REQUIRE(tree["key3"].size() == 3);
    REQUIRE(tree["key3"][0].as<int>() == 1);
    REQUIRE(tree["key3"][1].as<int>() == 2);
    REQUIRE(tree["key3"][2].as<int>() == 3);

    REQUIRE(tree["key4"].is_object());
    REQUIRE(tree["key4"]["nestedKey"].is_double());
    REQUIRE(tree["key4"]["nestedKey"].as<double>() == Catch::Approx(3.1415));

    REQUIRE(tree["key5"].is_null());

    REQUIRE(tree.has_key("invalid") == false);
    REQUIRE(tree["invalid"].is_empty());
}

TEST_CASE("JsonDataProcessor: JSON parsing with comments", "[JsonDataProcessor]")
{
    auto tree = from_json_string(
R"({
    /* ignored comment */
    "key1": "value1",
    "key2": 42
})");

    REQUIRE(tree["key1"].is_string());
    REQUIRE(tree["key1"].as<std::string>() == "value1");

    REQUIRE(tree["key2"].is_number());
    REQUIRE(tree["key2"].as<int>() == 42);
}

TEST_CASE("JsonDataProcessor: JSON parsing with escape sequences", "[JsonDataProcessor]")
{
    auto tree = from_json_string(
R"({
    "key1": "\nvalue1\t",
    "key2": "\"value\\2\"",
    "key3": "\u0032\u0034"
})");

    REQUIRE(tree["key1"].is_string());
    REQUIRE(tree["key1"].as<std::string>() == "\nvalue1\t");

    REQUIRE(tree["key2"].is_string());
    REQUIRE(tree["key2"].as<std::string>() == "\"value\\2\"");

    REQUIRE(tree["key3"].is_string());
    REQUIRE(tree["key3"].as<std::string>() == " \"");
}

TEST_CASE("JsonDataProcessor: JSON parsing with errors", "[JsonDataProcessor]")
{
    REQUIRE_THROWS(from_json_string(R"({"key1": "value1", "key2": 42, )")); // Trailing comma
    REQUIRE_THROWS(from_json_string(R"({"key1": "value1" "key2": 42})")); // Missing comma
    REQUIRE_THROWS(from_json_string(R"({"key1": "value1", "key2": [1, 2, })")); // Trailing comma in array
    REQUIRE_THROWS(from_json_string(R"({"key1": "value1", "key2": 42)")); // Missing closing brace
}

TEST_CASE("JsonDataProcessor: JSON serialization", "[JsonDataProcessor]")
{
    auto tree = DataTree::make_object();

    tree["key1"] = "value1";
    tree["key2"] = 42;
    tree["key3"] = DataTree::Array{1, 2, 3};
    tree["key4"] = DataTree::Object{{"nestedKey", nullptr}};
    tree["key5"] = nullptr;

    std::string expected_json =
R"({
  "key1": "value1",
  "key2": 42,
  "key3": [
    1,
    2,
    3
  ],
  "key4": {
    "nestedKey": null
  },
  "key5": null
})";

    auto json_str = to_json_string(tree, 2);
    REQUIRE(json_str == expected_json);
}

#endif
