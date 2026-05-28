/**
 * \file  OverloadSet.h
 * \brief Implementation of the OverloadSet class template.
 * \date  Created on 15 June 2023
 *
 * \copyright Copyright 2023-2025 Deutsches Elektronen-Synchrotron (DESY), Hamburg
 *
 * Distributed under the Boost Software License, Version 1.0 (see \ref license_boost_1_0
 * or https://www.boost.org/LICENSE_1_0.txt).
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
