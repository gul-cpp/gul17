/**
 * \file  trim.cc
 * \date  Created on November 22, 2018
 * \brief Example for the trim() family of functions from the General Utility Library.
 *
 * \copyright Copyright 2018-2019, 2024 Deutsches Elektronen-Synchrotron (DESY), Hamburg
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
