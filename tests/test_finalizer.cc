/**
 * \file  test_finalizer.cc
 * \brief Test suite for hexdump() and hexdump_stream().
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

#include <functional>
#include <new>
#include <string>

#include <catch2/catch_test_macros.hpp>

#include "gul17/finalizer.h"

TEST_CASE("Finalizer Tests", "[finalizer]")
{
    SECTION("Closure as temporary") {
        int foo = 1;
        {
            auto _ = gul17::finally([&foo] { foo += 2; });
        }
        REQUIRE(foo == 3);
    }

    SECTION("Closure as lvalue") {
        int foo = 1;
        {
            // materialized and copied
            auto xxx = [&foo] { foo += 2; };
            auto yyy { xxx };

            auto _ = gul17::finally(yyy);
        }
        REQUIRE(foo == 3);
    }

    SECTION("Closure as lvalue 2") {
        // Same test as before, but we want to check if the closure is really copied
        // by the code we use, so we need an closure internal memory ('called')
        int foo = 1;
        {
            // materialized and copied
            auto xxx = [&foo, called = 0]() mutable { foo += 2; ++called; return called; };
            REQUIRE(xxx() == 1);
            foo = 1;
            {
                auto yyy{ xxx };
                auto _ = gul17::finally(yyy);
            }
            REQUIRE(foo == 3);
            REQUIRE(xxx() == 2);
        }
        REQUIRE(foo == 5);
    }

    SECTION("Direct closure as lvalue") {
        int foo = 1;
        {
            // Use FinalAction directly
            auto xxx = [&foo] { foo += 2; };
            auto yyy { xxx };

            auto _ = gul17::FinalAction<decltype(yyy)>(yyy);
        }
        REQUIRE(foo == 3);
    }

    SECTION("Call on exception") {
        int foo = 1;
        try {
            auto _ = gul17::finally([&foo] { foo += 2; });
            throw "Foo bar";
        } catch (...) {
            foo++;
        }
        REQUIRE(foo == 4);
    }

    SECTION("String filling example") {
        std::string some_string {};
        char* buffer = nullptr;
        auto some_float = 123.45f;
        do {
            buffer = new char[100];
            if (buffer == nullptr)
                break;
            auto _ = gul17::finally([&buffer] { delete[] buffer; buffer = nullptr; });

            snprintf(buffer, 100, "%.1f", some_float);
            some_string = buffer;
            // get rid of buffer automagically
        }
        while (false);

        REQUIRE(some_string == "123.4");
        REQUIRE(buffer == nullptr);

    }

    SECTION("Closure move") {
        int foo = 1;
        {
            auto _1 = gul17::finally([&foo]() { foo += 2; });
            {
                auto _2 = std::move(_1);
                REQUIRE(foo == 1);
            }
            CHECK(foo == 3);
            {
                auto _2 = std::move(_1);
                REQUIRE(foo == 3);
            }
            REQUIRE(foo == 3);
        }
        REQUIRE(foo == 3);
    }
}

// Some preparation for function use-case
static auto global_foo{ 0 };

static void helper() {
    global_foo += 2;
}

static void helper2(int& foo) {
    foo += 2;
}

TEST_CASE("Finalizer with function", "[finalizer]")
{
    SECTION("Function pointer temporary") {
        global_foo = 1;
        {
            auto _ = gul17::finally(&helper);
        }
        REQUIRE(global_foo == 3);
    }

    SECTION("Function pointer lvalue") {
        global_foo = 1;
        {
            auto x = &helper;
            auto _ = gul17::finally(x);
        }
        REQUIRE(global_foo == 3);
    }

    SECTION("Assignment operator test") {
        global_foo = 1;
        {
            auto x = &helper;
            auto a = gul17::finally(x);
            {
                auto b = gul17::finally(x);
                b = std::move(a);
                REQUIRE(global_foo == 1);
            }
            REQUIRE(global_foo == 3);
        }
        REQUIRE(global_foo == 3);
    }

    SECTION("Function decays to function pointer temporary") {
         global_foo = 1;
         {
             // Lazy man's version, implicit conversion to function pointer
             auto _ = gul17::finally(helper);
         }
         REQUIRE(global_foo == 3);
    }

    SECTION("Direct function pointer temporary") {
        global_foo = 1;
        {
            // Use FinalAction directly
            auto _ = gul17::FinalAction<decltype(&helper)>(helper);
        }
        REQUIRE(global_foo == 3);
    }

    SECTION("Direct function pointer lvalue") {
        global_foo = 1;
        {
            // Use FinalAction directly
            auto x = &helper;
            auto _ = gul17::FinalAction<decltype(x)>(helper);
        }
        REQUIRE(global_foo == 3);
    }

    SECTION("Direct function decays to function pointer temporary") {
        global_foo = 1;
        {
            // Use FinalAction directly
            auto _ = gul17::FinalAction<decltype(&helper)>(helper);
        }
        REQUIRE(global_foo == 3);
    }

    SECTION("finally with std::bind") {
        int foo = 1;
        {
            auto _ = gul17::finally(std::bind(&helper2, std::ref(foo)));
            CHECK(foo == 1);
        }
        CHECK(foo == 3);
    }
}

// vi:et:sts=4:sw=4:ts=4
