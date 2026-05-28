/**
 * \file  test_type_name.cc
 * \date  Created on April 11, 2019
 * \brief Test suite for type_name().
 *
 * \copyright Copyright 2019-2025 Deutsches Elektronen-Synchrotron (DESY), Hamburg
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

#include <sstream>

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

#include "gul17/type_name.h"

using namespace std::literals::string_literals;
using Catch::Matchers::ContainsSubstring;

template <typename T>
class clever {
public:
    explicit clever(T) {}
};

TEST_CASE("Type-name Test", "[type_name]")
{
    SECTION("test some stdlib types") {
        auto oss = std::ostringstream{ };
        oss << gul17::type_name<decltype(oss)>();
        REQUIRE_THAT(oss.str(), ContainsSubstring("ostringstream"));

        oss.str("");
        oss << gul17::type_name<std::string>();
        REQUIRE_THAT(oss.str(),
            ContainsSubstring("basic_string") || ContainsSubstring("std::string"));

        oss.str("");
        oss << gul17::type_name<std::size_t>();
        REQUIRE_THAT(oss.str(), ContainsSubstring("unsigned"));
        REQUIRE_THAT(oss.str(), ContainsSubstring("int") or ContainsSubstring("long"));

        // Check that the indices on __PRETTY_FUNCTION__ are correct
        REQUIRE_THAT(oss.str(), not ContainsSubstring("["));
        REQUIRE_THAT(oss.str(), not ContainsSubstring("]"));
    }
    SECTION("test some basic types") {
        auto oss = std::ostringstream{ };
        oss << gul17::type_name<int>();
        REQUIRE_THAT(oss.str(), ContainsSubstring("int"));

        auto& x = "test";
        oss.str("");
        oss << gul17::type_name<decltype(x)>();
        REQUIRE_THAT(oss.str(), ContainsSubstring("const"));
        REQUIRE_THAT(oss.str(), ContainsSubstring("char"));
        REQUIRE_THAT(oss.str(), ContainsSubstring("[5]"));

        oss.str("");
        oss << gul17::type_name<std::decay_t<decltype(x)>>();
        REQUIRE_THAT(oss.str(), ContainsSubstring("const"));
        REQUIRE_THAT(oss.str(), ContainsSubstring("char"));
        REQUIRE_THAT(oss.str(), ContainsSubstring("*"));

        auto f = 2.3f;
        oss.str("");
        oss << gul17::type_name<decltype(f)>();
        REQUIRE_THAT(oss.str(), ContainsSubstring("float"));
    }
    SECTION("test some user types") {
        auto c = clever<int>{ 1 };
        auto oss = std::ostringstream{ };
        oss << gul17::type_name<decltype(c)>();
        REQUIRE_THAT(oss.str(), ContainsSubstring("clever"));
        REQUIRE_THAT(oss.str(), ContainsSubstring("int"));
    }
    SECTION("test constexpr-ness") {
        auto constexpr x = gul17::type_name<int>();
        auto oss = std::ostringstream{ };
        oss << x;
        REQUIRE_THAT(oss.str(), ContainsSubstring("int"));
    }

    SECTION("test compile type output") {
        // There is not much checking here, because the type
        // of 'c' will be output in a compiler error message,
        // when the following is not commented out:

        // auto c = clever<int>{ 1 };
        // gul17::FailToInstantiate<decltype(c)>{};
    }
}

// vi:ts=4:sw=4:et:sts=4
