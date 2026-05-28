/**
 * \file  time_util.h
 * \date  Created on September 7, 2018
 * \brief Declaration of time related functions for the General Utility Library.
 *
 * \copyright Copyright 2018-2023 Deutsches Elektronen-Synchrotron (DESY), Hamburg
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

#ifndef GUL17_TIME_UTIL_H_
#define GUL17_TIME_UTIL_H_

#include <chrono>
#include <thread>

#include "gul17/internal.h"
#include "gul17/Trigger.h"

namespace gul17 {

/**
 * \addtogroup time_util_h gul17/time_util.h
 * \brief Time-related functions.
 * @{
 */

/**
 * Return the current time as a std::chrono time_point.
 * This function is intended to be used with the sister function toc() to measure elapsed
 * time.
 *
 * <h4>Example</h4>
 * \code
 * auto t0 = tic();
 * // Do some work
 * std::cout << "Elapsed time: " << toc(t0) << " seconds.\n";
 * \endcode
 *
 * \see toc()
 */
inline std::chrono::steady_clock::time_point tic()
{
    return std::chrono::steady_clock::now();
}

/**
 * Return the elapsed time in seconds (or a different unit) since the given time point.
 * This function is intended to be used with the sister function tic() to measure elapsed
 * time. toc() is a function template that returns the elapsed seconds as a double value
 * by default; by specifying a different \c chrono type as a template parameter, it can
 * also return other time units and other types.
 *
 * <h4>Example</h4>
 * \code
 * auto t0 = tic();
 *
 * // <do some work>
 *
 * // Default: Return seconds as a double
 * std::cout << "Elapsed time: " << toc(t0) << " seconds.\n";
 *
 * // Custom type: Return milliseconds as an integer
 * std::cout << "Elapsed time: " << toc<std::chrono::milliseconds>(t0) << " milliseconds.\n";
 * \endcode
 *
 * \tparam TimeUnitType  The type to be used for calculating the elapsed time since t0.
 *     By default, this is std::chrono::duration<double>, which means that the elapsed
 *     time is returned as a double that represents seconds.
 *
 * \param t0  A time point in the past that should be taken with tic().
 * \returns the elapsed time in the units and base representation of TimeUnitType. By
 *     default, this is a double that represents elapsed seconds. For a TimeUnitType of
 *     std::chrono::milliseconds, it would be an integer representing elapsed
 *     milliseconds.
 *
 * \see tic()
 */
template<class TimeUnitType = std::chrono::duration<double>>
auto toc(std::chrono::steady_clock::time_point t0)
{
    return std::chrono::duration_cast<TimeUnitType>(tic() - t0).count();
}

/**
 * Sleep for at least the given time span, with the option of being woken up from another
 * thread. The sleep can be interrupted from another thread via a shared \ref Trigger
 * object.
 *
 * Calling sleep() may lead to a context switch of the operation system. Under heavy load
 * or resource contention, this can produce a delay that is longer than expected.
 *
 * \param duration   Time span to wait, as a std::chrono::duration type.
 * \param trg        Reference to a SleepInterrupt object that can be used to interrupt
 *                   the delay. If such an interruption occurs, false is returned.
 * \returns true if the entire requested sleep duration has passed, or false if the sleep
 *          has been interrupted prematurely via the Trigger object.
 *
 * \see \ref Trigger
 */
template< class Rep, class Period >
bool sleep(const std::chrono::duration<Rep, Period>& duration, const Trigger& trg)
{
    return !trg.wait_for(duration);
}

/**
 * Sleep for a given number of seconds, with the option of being woken up from another
 * thread. The sleep can be interrupted from another thread via a shared \ref Trigger
 * object.
 *
 * Calling sleep() may lead to a context switch of the operation system. Under heavy load
 * or resource contention, this can produce a delay that is longer than expected.
 *
 * \param seconds    Seconds to wait.
 * \param trg        Reference to a SleepInterrupt object that can be used to interrupt
 *                   the delay. If such an interruption occurs, false is returned.
 * \returns true if the entire requested sleep duration has passed, or false if the sleep
 *          has been interrupted prematurely via the Trigger object.
 *
 * \see \ref Trigger
 */
inline bool sleep(double seconds, const Trigger &trg)
{
    return sleep(std::chrono::duration<double>{ seconds }, trg);
}

/**
 * Sleep for a given time span.
 *
 * This is equivalent to a call of std::this_thread::sleep_for(), which means that control
 * is handed back to the operating system's task scheduler. This may result in a somewhat
 * bigger effective delay than expected, especially where very small sleep times are
 * requested and the system load is high.
 *
 * Example:
 * \code
 * using namespace std::literals; // for the "ms" suffix
 * sleep(50ms); // Wait 50 milliseconds
 * \endcode
 *
 * \param duration   Time span to wait, as a std::chrono::duration type.
 * \returns true to signalize that the entire requested sleep duration has passed. This is
 *          for symmetry with the interruptible version of sleep() only.
 */
template< class Rep, class Period >
bool sleep(const std::chrono::duration<Rep, Period>& duration)
{
    std::this_thread::sleep_for(duration);
    return true;
}

/**
 * Sleep for a given number of seconds.
 *
 * This is equivalent to a call of std::this_thread::sleep_for(), which means that control
 * is handed back to the operating system's task scheduler. This may result in a somewhat
 * bigger effective delay than expected, especially where very small sleep times are
 * requested and the system load is high.
 *
 * Example:
 * \code
 * sleep(0.05); // Wait 50 milliseconds
 * \endcode
 *
 * \param seconds  Seconds to wait.
 * \returns true to signalize that the entire requested sleep duration has passed. This is
 *          for symmetry with the interruptible version of sleep() only.
 */
inline bool sleep(double seconds)
{
    return sleep(std::chrono::duration<double>{ seconds });
}

/// @}

} // namespace gul17

#endif
