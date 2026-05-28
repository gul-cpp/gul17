/**
 * \file  gul.h
 * \date  Created on August 24, 2018
 * \brief Main include file for the General Utility Library.
 *
 * \copyright Copyright 2018-2025 Deutsches Elektronen-Synchrotron (DESY), Hamburg
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
