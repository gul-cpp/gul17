/**
 * \file    OverloadSet.h
 * \brief   Implementation of the OverloadSet class template and the make_overload_set()
 *          function template.
 * \authors \ref contributors
 * \date    Created on 15 June 2023
 *
 * \copyright Copyright 2023-2025 Deutsches Elektronen-Synchrotron (DESY), Hamburg
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
 * \since GUL14 version 2.9.2; dropped in GUL17 25.3.0; reintroduced in GUL17 version
 *        UNRELEASED
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
