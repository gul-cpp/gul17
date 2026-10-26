/**
 * \file  ThreadSafeQueue.h
 * \date  Created on April 1, 2022
 * \brief Declaration of the ThreadSafeQueue class.
 *
 * \copyright Copyright 2022-2026 Deutsches Elektronen-Synchrotron (DESY), Hamburg
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

#ifndef GUL17_THREADSAFEQUEUE_H_
#define GUL17_THREADSAFEQUEUE_H_

#include <condition_variable>
#include <mutex>
#include <optional>

#include "gul17/SlidingBuffer.h"

namespace gul17 {

/**
 * A strictly bounded thread-safe message queue.
 *
 * Messages are added to the end of the queue with push() and taken from the start of the
 * queue with pop(). These calls block if the queue is full or if no messages are
 * available, respectively. try_push() and try_pop() are non-blocking alternatives.
 *
 * \code{.cpp}
 * // Set up queue with capacity for 10 integers
 * ThreadSafeQueue<int> queue{ 10 };
 *
 * // Start sender thread, pushing 100 integers into the queue
 * std::thread sender([&queue]()
 *     {
 *         for (int i = 1; i <= 100; ++i)
 *             queue.push(i);
 *     });
 *
 * // Pull all 100 integers out of the queue in the main thread
 * for (int i = 1; i <= 100; ++i)
 * {
 *     int val = queue.pop();
 *     assert(val == i);
 * }
 *
 * // Join the sender thread
 * sender.join();
 * \endcode
 */
template <typename MessageT>
class ThreadSafeQueue
{
public:
    using MessageType = MessageT;
    using message_type = MessageT;
    using SizeType = std::size_t;
    using size_type = SizeType;

    /// Construct a queue that is able to hold a given maximum number of entries.
    ThreadSafeQueue(SizeType capacity)
        : queue_(capacity)
    {
    }

    /**
     * Fetch the last message from the queue and return a copy of it.
     * The message is not removed from the queue.
     *
     * This call blocks until a message is available.
     */
    MessageType back() const
    {
        std::unique_lock lock{ mutex_ };

        if (queue_.empty())
            cv_message_available_.wait(lock, [this] { return not queue_.empty(); });

        auto msg = queue_.back();
        lock.unlock();
        return msg;
    }

    /// Return the maximal number of entries in the queue.
    SizeType capacity() const
    {
        std::lock_guard lock{ mutex_ };
        return queue_.capacity();
    }

    /// Determine whether the queue is empty.
    bool empty() const
    {
        std::lock_guard lock{ mutex_ };
        return queue_.empty();
    }

    /**
     * Remove a message from the front of the queue and return it.
     *
     * This call blocks until a message is available.
     *
     * \see try_pop()
     */
    MessageType pop()
    {
        std::unique_lock lock{ mutex_ };

        if (queue_.empty())
            cv_message_available_.wait(lock, [this] { return not queue_.empty(); });

        auto msg = std::move(queue_.front());
        queue_.pop_front();
        lock.unlock();
        cv_slot_available_.notify_one();
        return msg;
    }

    /**
     * Insert a message at the end of the queue.
     *
     * This call blocks until the queue has a free slot for the message.
     */
    template <typename MsgT,
        std::enable_if_t<std::is_convertible_v<MsgT, MessageType>, bool> = true>
    void push(MsgT&& msg)
    {
        std::unique_lock lock{ mutex_ };

        if (queue_.filled())
            cv_slot_available_.wait(lock, [this] { return not queue_.filled(); });

        queue_.push_back(std::forward<MsgT>(msg));
        lock.unlock();
        cv_message_available_.notify_one();
    }

    /// Return the number of messages in the queue.
    SizeType size() const
    {
        std::lock_guard lock{ mutex_ };
        return queue_.size();
    }

    /**
     * Remove a message from the front of the queue and return it.
     *
     * This call blocks until a message is available.
     *
     * \see try_pop()
     */
    std::optional<MessageType> try_pop()
    {
        std::unique_lock lock{ mutex_ };

        if (queue_.empty())
            return std::nullopt;

        auto msg = std::move(queue_.front());
        queue_.pop_front();
        lock.unlock();
        cv_slot_available_.notify_one();
        return msg;
    }

    /**
     * Try to insert a message at the end of the queue.
     *
     * This call returns true if the message was successfully enqueued or false if the
     * queue temporarily had no space to store the message.
     *
     * Messages given as an rvalue are only moved from if they can actually be inserted
     * into the queue.
     */
    template <typename MsgT,
        std::enable_if_t<std::is_convertible_v<MsgT, MessageType>, bool> = true>
    bool try_push(MsgT&& msg)
    {
        std::unique_lock lock{ mutex_ };

        if (queue_.filled())
            return false;

        queue_.push_back(std::forward<MsgT>(msg));
        lock.unlock();
        cv_message_available_.notify_one();
        return true;
    }

private:
    /// Mutex protecting all member variables
    mutable std::mutex mutex_;

    /// Condition variable, triggered when at least one message has been added to the queue.
    mutable std::condition_variable cv_message_available_;

    /// Condition variable, triggered when at least one slot in the queue has been freed.
    mutable std::condition_variable cv_slot_available_;

    gul17::SlidingBuffer<MessageType> queue_;
};

} // namespace gul17

#endif
