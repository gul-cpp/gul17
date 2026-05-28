/**
 * \file  test_cat.cc
 * \date  Created on August 30, 2018
 * \brief Test suite for cat() from the General Utility Library.
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
