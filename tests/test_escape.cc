/**
 * \file  test_escape.cc
 * \date  Created on August 31, 2018
 * \brief Part of test suite for string utility functions in the General Utility Library.
 *
 * \copyright Copyright 2018-2026 Deutsches Elektronen-Synchrotron (DESY), Hamburg
 *
 * Distributed under the Boost Software License, Version 1.0 (see \ref license_boost_1_0
 * or https://www.boost.org/LICENSE_1_0.txt).
 */

#include <random>

#include <catch2/catch_test_macros.hpp>

#include "gul17/cat.h"
#include "gul17/escape.h"

using namespace std::literals;

TEST_CASE("Compare escaped strings", "[escape]")
{
    REQUIRE(gul17::escape("foo bar baz"s) == "foo bar baz"s);

    // This shows the design idea quite nice:
    REQUIRE(gul17::escape("foo\rbar\nfoobar\tbaz\\qux\""s) ==
              R"(foo\rbar\nfoobar\tbaz\\qux\")"s);

    REQUIRE(gul17::escape("foo\abar\000baz"s) == R"(foo\x07bar\x00baz)"s);

    REQUIRE(gul17::escape("\xff") == "\\xff");
}

TEST_CASE("Compare unescaped strings", "[escape]")
{
    REQUIRE(gul17::unescape("foo bar baz"s) == "foo bar baz"s);

    REQUIRE(gul17::unescape(R"(foo\rbar\nfoobar\tbaz\\qux\")"s) ==
                "foo\rbar\nfoobar\tbaz\\qux\""s);

    REQUIRE(gul17::unescape(R"(foo\x07bar\x00baz)"s) == "foo\abar\000baz"s);

    REQUIRE(gul17::unescape("\\xff") == "\xff");

    auto const s = "foo\abar\000baz"s;
    REQUIRE(gul17::unescape(gul17::escape(s)) == s);
}

TEST_CASE("Check escaping and unescaping with random strings", "[escape]")
{
    std::random_device r;
    std::default_random_engine re(r());
    std::uniform_int_distribution<unsigned short> uniform_dist(0, 255);

    for (std::size_t len = 0; len < 100u; ++len)
    {
        std::string original(len, ' ');

        for (char &c : original)
            c = static_cast<char>(uniform_dist(re));

        auto escaped = gul17::escape(original);

        REQUIRE(original.length() <= escaped.length());

        auto unescaped = gul17::unescape(escaped);

        if (original != unescaped)
        {
            INFO(gul17::cat("Original \"", original, "\" != \"", unescaped,
                          "\" after escape/unescape"));

            std::string str = "Original bytes:              ";

            for (const char &c : original)
                str += gul17::cat("[", static_cast<int>(c), "] ");

            str += "\nBytes after escape/unescape: ";

            for (const char &c : unescaped)
                str += gul17::cat("[", static_cast<int>(c), "] ");

            FAIL(str);
        }
    }
}

// vi:ts=4:sw=4:sts=4:et
