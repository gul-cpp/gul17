/**
 * \file  traits.h
 * \brief Some metaprogramming traits for the General Utility Library.
 * \date  Created on 20 August 2021
 *
 * \copyright Copyright 2021-2024 Deutsches Elektronen-Synchrotron (DESY), Hamburg
 *
 * Distributed under the Boost Software License, Version 1.0 (see \ref license_boost_1_0
 * or https://www.boost.org/LICENSE_1_0.txt).
 */

#ifndef GUL17_TRAITS_H_
#define GUL17_TRAITS_H_

#include <functional> // for std::invoke on C++17 compilers
#include <type_traits>
#include <utility>

#include "gul17/internal.h"

namespace gul17 {

/**
 * \addtogroup traits_h gul17/traits.h
 * \brief Type traits and helpers for metaprogramming.
 * @{
 */

/**
 * Helper type trait object to determine if a type is a container.
 *
 * A container is identified by the presense of the ``cbegin()`` and ``cend()``
 * member functions and a ``value_type`` type alias.
 *
 * If the type specified as template parameter has the required members
 * ``std::true_type`` is returned, otherwise ``std::false_type``.
 *
 * \tparam T    Type to check
 * \returns     ``std::true_type`` if the type is probably a container
 *
 * \since GUL version 2.6
 */
template <typename T, typename = int>
struct IsContainerLike : std::false_type { };

template <typename T>
struct IsContainerLike <T,
    typename std::enable_if_t<true,
        decltype(std::declval<T>().cbegin(),
            std::declval<T>().cend(),
            std::declval<typename T::value_type>(),
            0)
    >>
    : std::true_type { };

/**
 * A template metafunction that removes const, volatile, and reference qualifiers from a
 * type. The stripped type is available in the member typedef \c type.
 *
 * This is a substitute for C++20's
 * [std::remove_cvref](https://en.cppreference.com/w/cpp/types/remove_cvref).
 *
 * \since GUL version 2.9
 */
template <typename T>
using remove_cvref = typename std::remove_cv<std::remove_reference_t<T>>;

/**
 * A template metafunction that removes const, volatile, and reference qualifiers from a
 * type.
 *
 * This is a substitute for C++20's
 * [std::remove_cvref_t](https://en.cppreference.com/w/cpp/types/remove_cvref_t).
 *
 * \since GUL version 2.9
 */
template <typename T>
using remove_cvref_t = typename remove_cvref<T>::type;

/// @}

} // namespace gul17

#endif

// vi:ts=4:sw=4:sts=4:et
