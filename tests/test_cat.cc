/**
 * \file  test_cat.cc
 * \date  Created on August 30, 2018
 * \brief Test suite for cat() from the General Utility Library.
 *
 * \copyright Copyright 2018-2025 Deutsches Elektronen-Synchrotron (DESY), Hamburg
 *
 * Distributed under the Boost Software License, Version 1.0 (see \ref license_boost_1_0
 * or https://www.boost.org/LICENSE_1_0.txt).
 */

 #include <catch2/catch_test_macros.hpp>

 #include "gul17/cat.h"

using namespace std::literals;
using gul17::cat;

TEST_CASE("gul17::cat() works for fixed test cases from 0 to 6 arguments", "[cat]")
{
    REQUIRE(cat() == "");
    REQUIRE(cat("Lo") == "Lo");
    REQUIRE(cat("Lu", "la") == "Lula");
    REQUIRE(cat("La", "le", "lu") == "Lalelu");
    REQUIRE(cat("La", "le", "lu", 42) == "Lalelu42");
    REQUIRE(cat("La", "le", "lu", 42, '#') == "Lalelu42#");
    REQUIRE(cat("La", "le", "lu", 42, " # ", 6.0) == "Lalelu42 # 6.000000");
}

// vi:ts=4:sw=4:sts=4:et
