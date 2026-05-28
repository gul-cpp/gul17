/**
 * \file  Trigger.cc
 * \brief Implementation of the Trigger class for the General Utility Library.
 * \date  Created on September 21, 2018
 *
 * \copyright Copyright 2018 Deutsches Elektronen-Synchrotron (DESY), Hamburg
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

#include "gul17/Trigger.h"

namespace gul17 {


Trigger::~Trigger() noexcept
{
    trigger();
}

Trigger &Trigger::operator=(bool interrupt) noexcept
{
    if (interrupt)
    {
        this->trigger();
    }
    else
    {
        std::lock_guard<std::mutex> lock(mutex_);
        triggered_ = false;
    }

    return *this;
}

Trigger::operator bool() const noexcept
{
    std::lock_guard<std::mutex> lock(mutex_);
    return triggered_;
}

void Trigger::reset() noexcept
{
    std::lock_guard<std::mutex> lock(mutex_);
    triggered_ = false;
}

void Trigger::trigger() noexcept
{
    {
        std::lock_guard<std::mutex> lock(mutex_);
        triggered_ = true;
    }

    // It is more efficient if we do not hold the lock on mutex_ when notifying other
    // threads, because they need to acquire the lock as well.
    cv_.notify_all();
}

void Trigger::wait() const
{
    std::unique_lock<std::mutex> lock(mutex_);
    cv_.wait(lock, [this]{ return triggered_; });
}


} // namespace gul17
