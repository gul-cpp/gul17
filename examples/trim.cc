/**
 * \file  trim.cc
 * \date  Created on November 22, 2018
 * \brief Example for the trim() family of functions from the General Utility Library.
 *
 * \copyright Copyright 2018-2019, 2024 Deutsches Elektronen-Synchrotron (DESY), Hamburg
 *
 * Distributed under the Boost Software License, Version 1.0 (see \ref license_boost_1_0
 * or https://www.boost.org/LICENSE_1_0.txt).
 */

#include <iostream>
#include <gul17/trim.h>

using gul17::trim_left_sv;
using gul17::trim_left;
using gul17::trim_right_sv;
using gul17::trim_right;
using gul17::trim_sv;
using gul17::trim;
using std::cout;

int main()
{
//! [Using trim() with default and custom whitespace]
    cout << "[" << trim("\n \b trim(), default whitespace\t \r") << "]\n";
    // prints "[trim(), default whitespace]"

    cout << "[" << trim(".:.:.:trim(), custom whitespace.:.:.:.", ".:") << "]\n";
    // prints "[trim(), custom whitespace]"
//! [Using trim() with default and custom whitespace]

//! [Using trim_left() and trim_right() with default and custom whitespace]
    cout << "[" << trim_left("\n \b trim_left(), default whitespace   ") << "]\n";
    // prints "[trim_left(), default whitespace   ]"

    cout << "[" << trim_right(".:.:.:trim_right(), custom whitespace.:.:.:.", ".:") << "]\n";
    // prints "[.:.:.:trim_right, custom whitespace]"
//! [Using trim_left() and trim_right() with default and custom whitespace]

//! [Using trim_left_sv() and trim_right_sv() with default and custom whitespace]
    cout << "[" << trim_left_sv("\n \b trim_left_sv(), default whitespace   ") << "]\n";
    // prints "[trim_left_sv(), default whitespace   ]"

    cout << "[" << trim_right_sv(".:.:.:trim_right_sv(), custom whitespace.:.:.:.", ".:") << "]\n";
    // prints "[.:.:.:trim_right_sv, custom whitespace]"
//! [Using trim_left_sv() and trim_right_sv() with default and custom whitespace]

//! [Using trim_sv() on a mutable string]
    std::string str = "  string_view  ";
    auto sv = trim_sv(str); // sv is a string_view that points to the original string

    cout << "[" << sv << "]\n";
    // prints "[string_view]"

    str[5] = 'o'; // modify the original string

    cout << "[" << sv << "]\n";
    // prints "[strong_view]"
//! [Using trim_sv() on a mutable string]

    return 0;
}
