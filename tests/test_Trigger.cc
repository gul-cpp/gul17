/**
 * \file   test_Trigger.cc
 * \author \ref contributors
 * \date   Created on September 21, 2018
 * \brief  Test suite for the Trigger class from the General Utility Library.
 *
 * \copyright Copyright 2018-2025 Deutsches Elektronen-Synchrotron (DESY), Hamburg
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

#include <future>

#include <catch2/catch_test_macros.hpp>

#include "gul17/time_util.h"
#include "gul17/Trigger.h"

using namespace std::literals;

using gul17::tic;
using gul17::toc;
using gul17::sleep;
using gul17::Trigger;

TEST_CASE("Construction, assignment, equality and bool operator work", "[Trigger]")
{
    Trigger trg;
    REQUIRE(trg == false);
    trg = true;
    REQUIRE(trg == true);

    Trigger trg2{ true };
    REQUIRE(trg2 == true);
    trg2 = false;
    REQUIRE(trg2 == false);
}

SCENARIO("Trigger::wait*() resumes if another thread calls trigger()", "[Trigger]")
{
    GIVEN("a trigger and a helper thread that sets it to true after 50 ms")
    {
        Trigger trg;
        std::chrono::steady_clock::time_point t0;

        auto future = std::async(std::launch::async,
                [&t0, &trg]
                {
                    t0 = tic();
                    sleep(50ms);
                    trg = true;
                });

        WHEN("the main thread calls wait()")
        {
            trg.wait();
            auto time_ms = toc<std::chrono::milliseconds>(t0);

            THEN("it resumes after >= 50 ms")
            {
                REQUIRE(time_ms >= 50);
            }
        }

        WHEN("the main thread calls wait_for(1s)")
        {
            trg.wait_for(1s);
            auto time_ms = toc<std::chrono::milliseconds>(t0);

            THEN("it resumes after >= 50 ms")
            {
                REQUIRE(time_ms >= 50);
            }
        }

        WHEN("the main thread calls wait_until(now + 5s)")
        {
            trg.wait_until(std::chrono::system_clock::now() + 5s);
            auto time_ms = toc<std::chrono::milliseconds>(t0);

            THEN("it resumes after >= 50 ms, but before 5 s")
            {
                REQUIRE(time_ms >= 50);
                REQUIRE(time_ms <= 5000);
            }
        }
    }
}

SCENARIO("Trigger::wait_for() and wait_until() wait at least the requested amount of time",
         "[Trigger]")
{
    Trigger trg;

    auto t0 = tic();

    WHEN("calling wait_for(50ms)")
    {
        trg.wait_for(50ms);
        auto time_ms = toc<std::chrono::milliseconds>(t0);

        THEN("execution resumes after >= 50 ms")
        {
            REQUIRE(time_ms >= 50);
        }
    }

    WHEN("calling wait_until(now + 50ms)")
    {
        trg.wait_until(std::chrono::system_clock::now() + 50ms);
        auto time_ms = toc<std::chrono::milliseconds>(t0);

        THEN("execution resumes after >= 50 ms")
        {
            REQUIRE(time_ms >= 50);
        }
    }
}

// vi:ts=4:sw=4:sts=4:et
