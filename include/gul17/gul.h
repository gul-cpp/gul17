/**
 * \file  gul.h
 * \date  Created on August 24, 2018
 * \brief Main include file for the General Utility Library.
 *
 * \copyright Copyright 2018-2025 Deutsches Elektronen-Synchrotron (DESY), Hamburg
 *
 * Distributed under the Boost Software License, Version 1.0 (see \ref license_boost_1_0
 * or https://www.boost.org/LICENSE_1_0.txt).
 */

#ifndef GUL17_GUL_H_
#define GUL17_GUL_H_

/**
 * \defgroup gul_h gul17/gul.h
 * \brief All-in-one include file for GUL17.
 * \details
 * This file includes almost all other headers provided by the library. One specific file
 * is excluded, however:
 * <ul>
 * <li>`gul17/date.h`: Howard Hinnant's header for calendar date functionality</li>
 * </ul>
 * `date.h` is excluded chiefly because of its negative impact on compile times. Just
 * include it directly where needed.
 */

#include "gul17/bit_manip.h"
#include "gul17/case_ascii.h"
#include "gul17/cat.h"
// #include "gul17/date.h" not included by default to reduce compile times
#include "gul17/escape.h"
#include "gul17/expected.h"
#include "gul17/finalizer.h"
#include "gul17/gcd_lcm.h"
#include "gul17/hexdump.h"
#include "gul17/join_split.h"
#include "gul17/num_util.h"
#include "gul17/OverloadSet.h"
#include "gul17/replace.h"
#include "gul17/SlidingBuffer.h"
#include "gul17/SmallVector.h"
#include "gul17/span.h"
#include "gul17/statistics.h"
#include "gul17/string_util.h"
#include "gul17/substring_checks.h"
#include "gul17/ThreadPool.h"
#include "gul17/time_util.h"
#include "gul17/to_number.h"
#include "gul17/tokenize.h"
#include "gul17/traits.h"
#include "gul17/Trigger.h"
#include "gul17/trim.h"
#include "gul17/type_name.h"
#include "gul17/version.h"

#endif
