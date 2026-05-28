/**
 * \file  OverloadSet.h
 * \brief Implementation of the OverloadSet class template.
 * \date  Created on 15 June 2023
 *
 * \copyright Copyright 2023-2025 Deutsches Elektronen-Synchrotron (DESY), Hamburg
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

#ifndef GUL17_OVERLOADSET_H_
#define GUL17_OVERLOADSET_H_

namespace gul17 {

/**
 * \addtogroup variant_h gul17/variant.h
 * \brief Implementation of the OverloadSet class template.
 * @{
 */

/**
 * A function object that works like an overload set of functions.
 *
 * \code{.cpp}
 * std::variant<int, double> v = 1.0;
 * auto str = std::visit(
 *     gul17::OverloadSet{
 *         [](char) { return "char"; },
 *         [](int) { return "int"; }
 *     }, v);
 * \endcode
 *
 * This variadic class template inherits from an arbitrary number of function objects
 * (typically lambdas) and imports their definitions of `operator()` into an overload set
 * for its own `operator()`. This is primarily helpful for use with std::visit().
 *
 * A similar class was proposed in B. Stroustrup's "A Tour of C++", 3rd ed.,
 * Addison-Wesley, 2023.
 *
 * \since GUL14 version 2.9.2; dropped in GUL17 25.3.0; reintroduced in GUL17 version
 *        25.7.0
 */
template<typename... Fcts>
struct OverloadSet : Fcts ...
{
    using Fcts::operator()...;
};

template<typename... Fcts>
OverloadSet(Fcts...) -> OverloadSet<Fcts...>;

} // namespace gul17

#endif
