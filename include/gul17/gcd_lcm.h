/**
 * \file  gcd_lcm.h
 * \brief Declarations of gcd() and lcm().
 * \date  Created on 5 August 2022
 *
 * \copyright Copyright 2022-2023 Deutsches Elektronen-Synchrotron (DESY), Hamburg
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

#ifndef GUL17_GCD_LCM_H_
#define GUL17_GCD_LCM_H_

#include <type_traits>

#include "gul17/internal.h"
#include "gul17/num_util.h"

namespace gul17 {

/**
 * \addtogroup gcd_lcm_h gul17/gcd_lcm.h
 * \brief Greatest common divisor and least common multiple.
 * @{
 */

/**
 * Calculate the greatest common divisor of two integers using the Euclidean algorithm.
 *
 * If both numbers are zero, the function returns zero. Otherwise, the result is a
 * positive integer.
 *
 * \code
 * int greatest_common_divisor = gcd(10, 15); // returns 5
 * \endcode
 *
 * \returns the least common multiple of the two numbers, represented as an integer type
 *          (`std::common_type_t<IntTypeA, IntTypeB>`) that both inputs can implicitly be
 *          converted to. If either a, b, or the result can not be represented by that
 *          type, the result is undefined.
 *
 * \note
 * Unlike `std::gcd()` from C++17, the GUL17 version cannot be used with integers of
 * different signedness. This avoids undefined behavior when mixing unsigned integers with
 * negative signed values:
 * \code
 * // C++17 gcd(): Undefined behavior - the common type is `unsigned int` and -5 has no
 * // representation in that type
 * auto bad_result = std::gcd(10u, -5);
 *
 * // GUL17 gcd(): Does not compile
 * auto static_assertion_failure = gul17::gcd(10u, -5);
 * \endcode
 *
 * \since GUL version 2.7
 */
template <typename IntTypeA, typename IntTypeB>
constexpr inline auto
gcd(IntTypeA a, IntTypeB b)
{
    static_assert(std::is_integral<IntTypeA>::value and std::is_integral<IntTypeB>::value,
        "gcd() arguments must be integers");

    static_assert(std::is_signed<IntTypeA>::value == std::is_signed<IntTypeB>::value,
        "gcd() arguments must have the same signedness");

    using CommonType = std::common_type_t<IntTypeA, IntTypeB>;

    auto c = gul17::abs(static_cast<CommonType>(a));
    auto d = gul17::abs(static_cast<CommonType>(b));

    while (d != 0)
    {
        CommonType tmp = d;
        d = c % d;
        c = tmp;
    }

    return c;
}

/**
 * Calculate the least common multiple of two integers.
 *
 * If both numbers are zero, the function returns zero. Otherwise, the result is a
 * positive integer.
 * \code
 * int least_common_multiple = lcm(10, 15); // returns 30
 * \endcode
 *
 * \returns the least common multiple of the two numbers, represented as an integer type
 *          (`std::common_type_t<IntTypeA, IntTypeB>`) that both inputs can implicitly be
 *          converted to. If either a, b, or the result can not be represented by that
 *          type, the result is undefined.
 *
 * \note
 * Unlike `std::lcm()` from C++17, the GUL17 version cannot be used with integers of
 * different signedness. This avoids undefined behavior when mixing unsigned integers with
 * negative signed values:
 * \code
 * // C++17 lcm(): Undefined behavior - the common type is `unsigned int` and -5 has no
 * // representation in that type
 * auto bad_result = std::lcm(10u, -5);
 *
 * // GUL17 lcm(): Does not compile
 * auto static_assertion_failure = gul17::lcm(10u, -5);
 * \endcode
 *
 * \since GUL version 2.7
 */
template <typename IntTypeA, typename IntTypeB>
constexpr inline auto
lcm(IntTypeA a, IntTypeB b)
{
    static_assert(std::is_integral<IntTypeA>::value and std::is_integral<IntTypeB>::value,
        "lcm() arguments must be integers");

    static_assert(std::is_signed<IntTypeA>::value == std::is_signed<IntTypeB>::value,
        "lcm() arguments must have the same signedness");

    using CommonType = std::common_type_t<IntTypeA, IntTypeB>;

    if (a == 0 && b == 0)
        return CommonType{ 0 };

    return static_cast<CommonType>(gul17::abs((a / gcd(a, b)) * b));
}

/// @}

} // namespace gul17

#endif
