/**
 * \file  Trigger.cc
 * \brief Implementation of the Trigger class for the General Utility Library.
 * \date  Created on September 21, 2018
 *
 * \copyright Copyright 2018 Deutsches Elektronen-Synchrotron (DESY), Hamburg
 *
 * Distributed under the Boost Software License, Version 1.0 (see \ref license_boost_1_0
 * or https://www.boost.org/LICENSE_1_0.txt).
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
