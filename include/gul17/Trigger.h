/**
 * \file  Trigger.h
 * \date  Created on September 21, 2018
 * \brief Declaration of the Trigger class for the General Utility Library.
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

#ifndef GUL17_TRIGGER_H_
#define GUL17_TRIGGER_H_

#include <chrono>
#include <condition_variable>
#include <mutex>

#include "gul17/internal.h"

namespace gul17 {

/**
 * \addtogroup Trigger_h gul17/Trigger.h
 * \brief A cross-thread trigger.
 * @{
 */

/**
 * A class that allows sending triggers and waiting for them across different threads.
 *
 * A Trigger object is similar to a digital electric trigger signal. Its state is either
 * high (true) or low (false), and actions are triggered when it is at the high level -
 * that is to say, an arbitrary number of threads can wait until the Trigger object
 * becomes true. Internally, a Trigger is little more than a wrapper around a mutex, a
 * condition variable, and a boolean.
 *
 * There are two ways to use a Trigger:
 *
 * <h3>Using bools</h3>
 *
 * To stay in the picture of a digital electric trigger line, high and low states can
 * be assigned and tested like booleans. This allows for easy communication of state:
 * \code
 * // Data shared between threads
 * Trigger trigger;
 *
 * ...
 *
 * // Thread 1
 * trigger.wait(); // wait until trigger becomes true
 * while (trigger)
 * {
 *     cout << "high\n";
 *     sleep(10ms);
 * }
 * cout << "low\n";
 * ...
 *
 * // Thread 2
 * trigger = true;
 * sleep(100ms);
 * trigger = false;
 * \endcode
 *
 * <h3>Using member functions</h3>
 *
 * Sometimes assignment of boolean values does not fit well to a specific task. The member
 * functions trigger() and reset() are identical to assigning \c true and \c false,
 * respectively:
 * \code
 * // Data shared between threads
 * Trigger trigger;
 *
 * ...
 *
 * // Thread 1
 * while (true)
 * {
 *     trigger.wait();
 *     cout << "Triggered!\n";
 *     trigger.reset();
 * }
 * ...
 *
 * // Thread 2
 * trigger.trigger();
 * sleep(100ms);
 * trigger.trigger();
 * sleep(100ms);
 * trigger.trigger();
 * \endcode
 * The unconditional wait() stops execution until the Trigger becomes true or is
 * destructed. If a timeout is required, wait_for() or wait_until() can be used instead.
 *
 * \note
 * Trigger is a thread-safe, self-synchronizing class; it is neither copyable nor movable.
 */
class Trigger
{
public:
    /// Constructor
    explicit Trigger(bool triggered = false) noexcept : triggered_{ triggered }
    {}

    /**
     * Destructor: Send a final trigger signal so that all threads waiting on this object
     * have a chance to stop.
     */
    GUL_EXPORT
    ~Trigger() noexcept;

    /// Not copyable and not movable
    Trigger(const Trigger&) = delete;
    Trigger(Trigger&&) = delete;
    Trigger& operator=(const Trigger&) = delete;
    Trigger& operator=(Trigger&&) = delete;

    /**
     * Return if the trigger is high==true or low==false.
     *
     * Example:
     * \code
     * Trigger t;
     * if (!t) // t returns false
     *     cout << "Trigger is low\n";
     * t.trigger();
     * if (t) // t now returns true
     *     cout << "Trigger is now high\n";
     * \endcode
     */
    GUL_EXPORT
    operator bool() const noexcept;

    /**
     * Set the trigger to high==true or low==false.
     * Setting it to true will cause any waiting threads to resume.
     */
    GUL_EXPORT
    Trigger& operator=(bool interrupt) noexcept;

    /// Set the trigger to low (false).
    GUL_EXPORT
    void reset() noexcept;

    /**
     * Set the trigger to high (true).
     * This causes any waiting threads to resume.
     */
    GUL_EXPORT
    void trigger() noexcept;

    /**
     * Suspend execution of the current thread until the trigger goes high (true).
     * Execution is also resumed if the object is destructed.
     */
    GUL_EXPORT
    void wait() const;

    /**
     * Suspend execution of the current thread until the trigger goes high (true) or at
     * least the given time span has passed.
     * Execution is also resumed if the object is destructed.
     *
     * \tparam Clock  The type of the underlying clock, e.g. std::chrono::system_clock.
     * \tparam Duration  The duration type to be used, typically Clock::duration.
     *
     * \param delta_t  A time span to wait. If the trigger still has not become true after
     *                 (at least) this time, the function returns false.
     *
     * \returns the state of the trigger at the end of the call. If this is false, the
     *          function has exited due to timeout.
     *
     * \note
     * For many applications, the free function sleep() is easier to use.
     */
    template <class Rep, class Period>
    bool wait_for(const std::chrono::duration<Rep, Period>& delta_t) const
    {
        std::unique_lock<std::mutex> lock(mutex_);
        return cv_.wait_for(lock, delta_t, [this]{ return triggered_; });
    }

    /**
     * Suspend execution of the current thread until the trigger goes high (true) or the
     * given time point has been passed.
     * Execution is also resumed if the object is destructed.
     *
     * \tparam Clock  The type of the underlying clock, e.g. std::chrono::system_clock.
     * \tparam Duration  The duration type to be used, typically Clock::duration.
     *
     * \param t  A time point after which the function should stop waiting.
     *
     * \returns the state of the trigger at the end of the call. If this is false, the
     *          function has exited due to timeout.
     *
     * \note
     * For many applications, the free function sleep() is easier to use.
     */
    template <class Clock, class Duration>
    bool wait_until(const std::chrono::time_point<Clock, Duration>& t) const
    {
        std::unique_lock<std::mutex> lock(mutex_);
        return cv_.wait_until(lock, t, [this]{ return triggered_; });
    }

private:
    mutable std::mutex mutex_; // Protects private data and is used with the condition variable
    mutable std::condition_variable cv_;
    bool triggered_ = false;
};

/// @}

} // namespace gul17

#endif
