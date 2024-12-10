/**
 * \file    gul.h
 * \authors \ref contributors
 * \date    Created on August 24, 2018
 * \brief   Main include file for the General Utility Library.
 *
 * \copyright Copyright 2018-2024 Deutsches Elektronen-Synchrotron (DESY), Hamburg
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

#ifndef GUL17_GUL_H_
#define GUL17_GUL_H_

/**
 * \defgroup gul_h gul17/gul.h
 * \brief All-in-one include file for GUL17.
 * \details
 * This file includes almost all other headers provided by the library. Two specific files
 * are excluded, however:
 * <ul>
 * <li>`gul17/catch.h`: The header file for the Catch2 unit test framework</li>
 * <li>`gul17/date.h`: Howard Hinnant's header for calendar date functionality</li>
 * </ul>
 * While the unit test framework is simply not useful for inclusion into regular code,
 * `date.h` is excluded chiefly because of its negative impact on compile times. Just
 * include it directly where needed.
 */

#include "gul17/bit_manip.h"
#include "gul17/case_ascii.h"
#include "gul17/cat.h"
// #include "gul17/catch.h" not included because it is only useful for unit tests
// #include "gul17/date.h" not included by default to reduce compile times
#include "gul17/escape.h"
#include "gul17/expected.h"
#include "gul17/finalizer.h"
#include "gul17/gcd_lcm.h"
#include "gul17/hexdump.h"
#include "gul17/join_split.h"
#include "gul17/num_util.h"
#include <optional>
#include "gul17/replace.h"
#include "gul17/SlidingBuffer.h"
#include "gul17/SmallVector.h"
#include "gul17/span.h"
#include "gul17/statistics.h"
#include "gul17/string_util.h"
#include <string_view>
#include "gul17/substring_checks.h"
#include "gul17/ThreadPool.h"
#include "gul17/time_util.h"
#include "gul17/to_number.h"
#include "gul17/tokenize.h"
#include "gul17/traits.h"
#include "gul17/Trigger.h"
#include "gul17/trim.h"
#include "gul17/type_name.h"
#include <variant>
#include "gul17/version.h"

#endif
