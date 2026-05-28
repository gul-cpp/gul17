/**
 * \file  test_main.cc
 * \date  Created on August 28, 2018
 * \brief Test suite for the General Utility Library.
 *
 * \copyright Copyright 2018-2025 Deutsches Elektronen-Synchrotron (DESY), Hamburg
 *
 * Distributed under the Boost Software License, Version 1.0 (see \ref license_boost_1_0
 * or https://www.boost.org/LICENSE_1_0.txt).
 */

#define CATCH_CONFIG_RUNNER

#include <catch2/catch_session.hpp>

#ifdef _MSC_VER
#   include <windows.h>
#endif

int main(int argc, char* argv[])
{
#ifdef _MSC_VER
    // On windows we often start with IDLE_PRIORITY_CLASS:
    //   Process whose threads run only when the system is idle.
    // That priority is used for batch scripts for example.
    //
    // This is of course not acceptable when we want to test some timing.
    SetPriorityClass(GetCurrentProcess(), ABOVE_NORMAL_PRIORITY_CLASS);
#endif
    return Catch::Session().run(argc, argv);
}

// vi:ts=4:sw=4:sts=4:et
