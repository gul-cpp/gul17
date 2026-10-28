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
#include <stdexcept>
#include <type_traits>
#include <utility>

#include "gul17/SlidingBuffer.h"

namespace gul17 {

/**
 * A strictly bounded thread-safe message queue for N producers and M consumers that is
 * not lock-free.
 *
 * The queue uses mutexes and condition variables to synchronize access between multiple
 * producers and consumers. It typically performs well under low contention (low numbers
 * of producers and consumers, reasonably low message rate) but may not be the optimal
 * choice for low-latency applications.
 *
 * Messages may be of any type that is default-constructible and movable. They are added
 * to the end of the queue with push() and taken from the start of the queue with pop().
 * These calls block if the queue is full or if no messages are available, respectively.
 * try_push() and try_pop() are non-blocking alternatives.
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
 *
 * For convenience, the queue also provides a back() member function that blocks until a
 * message is available and then returns a copy of the last message without removing it
 * from the queue. Obviously, use of back() requires the message type to be copyable.
 *
 * A ThreadSafeQueue is a synchronization tool for multiple threads. As such, it cannot be
 * copied or moved. It also has no built-in termination mechanism: The user must make sure
 * that all threads using the queue can shut down properly; typically, this is done by
 * sending one or more special termination messages through the queue.
 *
 * \since version UNRELEASED
 */
template <typename MessageT>
class ThreadSafeQueue
{
public:
    static_assert(std::is_default_constructible_v<MessageT>,
        "Message type must be default-constructible");
    static_assert(std::is_move_constructible_v<MessageT>,
        "Message type must be move-constructible");
    static_assert(
        std::is_move_assignable_v<MessageT>, "Message type must be move-assignable");

    using MessageType = MessageT;
    using message_type = MessageT;
    using SizeType = std::size_t;
    using size_type = SizeType;

    /**
     * Construct a queue that is able to hold a given maximum number of entries.
     * \param capacity  The maximum number of entries the queue can hold
     * \exception std::invalid_argument is thrown if the capacity is zero.
     */
    ThreadSafeQueue(SizeType capacity)
        : queue_(capacity)
    {
        if (capacity == 0)
        {
            throw std::invalid_argument(
                "ThreadSafeQueue capacity must be greater than 0");
        }
    }

    ~ThreadSafeQueue() = default;
    ThreadSafeQueue(const ThreadSafeQueue&) = delete;
    ThreadSafeQueue(ThreadSafeQueue&&) = delete;
    ThreadSafeQueue& operator=(const ThreadSafeQueue&) = delete;
    ThreadSafeQueue& operator=(ThreadSafeQueue&&) = delete;

    /**
     * Fetch the last message from the queue and return a copy of it.
     * The message is not removed from the queue.
     *
     * This call blocks until a message is available.
     *
     * \note
     * Obviously, this function requires the message to be copyable.
     */
    MessageType back() const
    {
        static_assert(std::is_copy_constructible_v<MessageT>,
            "Message type must be copy-constructible to use back()");

        std::unique_lock lock{ mutex_ };
        cv_message_available_.wait(lock, [this] { return not queue_.empty(); });

        return queue_.back();
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
     * <h3>Exception Safety</h3>
     * If an exception occurs while moving the message, the exception is rethrown. The
     * queue remains in a valid state, but it is unspecified whether the message was
     * actually removed from the queue or not. If that is a concern, use the
     * pop(MessageType&) overload instead.
     *
     * \see pop(MessageType&), try_pop()
     */
    MessageType pop()
    {
        std::unique_lock lock{ mutex_ };
        cv_message_available_.wait(lock, [this] { return not queue_.empty(); });

        auto msg = std::move(queue_.front());
        queue_.pop_front();
        lock.unlock();
        cv_slot_available_.notify_one();
        return msg;
    }

    /**
     * Remove a message from the front of the queue and return it via an output parameter.
     *
     * This call blocks until a message is available. Its main advantage over the
     * parameter-free pop() overload is better exception safety if the move assignment of
     * the message can throw. If that is not the case, prefer pop() for improved
     * readability.
     *
     * <h3>Exception Safety</h3>
     * If the move assignment of the message throws, the exception is rethrown. The
     * element remains in the queue but its state depends on the guarantees of the message
     * type.
     *
     * \see pop(), try_pop()
     */
    void pop(MessageType& msg)
    {
        std::unique_lock lock{ mutex_ };
        cv_message_available_.wait(lock, [this] { return not queue_.empty(); });

        msg = std::move(queue_.front());
        queue_.pop_front();
        lock.unlock();
        cv_slot_available_.notify_one();
    }

    /**
     * Insert a message at the end of the queue.
     *
     * This call blocks until the queue has a free slot for the message.
     *
     * <h3>Exception Safety</h3>
     * If the assignment of the message into the queue throws, the exception is rethrown
     * and no message is inserted.
     */
    template <typename MsgT,
        std::enable_if_t<std::is_convertible_v<MsgT, MessageType>, bool> = true>
    void push(MsgT&& msg)
    {
        std::unique_lock lock{ mutex_ };
        cv_slot_available_.wait(lock, [this] { return not queue_.filled(); });

        try
        {
            queue_.push_back(std::forward<MsgT>(msg));
        }
        catch (...)
        {
            // Forward the "slot available" notification to another waiting thread.
            lock.unlock();
            cv_slot_available_.notify_one();
            throw;
        }

        lock.unlock();
        cv_message_available_.notify_all();
    }

    /// Return the number of messages in the queue.
    SizeType size() const
    {
        std::lock_guard lock{ mutex_ };
        return queue_.size();
    }

    /**
     * If a message is available, remove it from the front of the queue and return it;
     * otherwise, return an empty optional.
     *
     * <h3>Exception Safety</h3>
     * If an exception occurs while moving the message, the exception is rethrown. The
     * queue remains in a valid state, but it is unspecified whether the message was
     * actually removed from the queue or not. If that is a concern, use the
     * try_pop(MessageType&) overload instead.
     *
     * \see try_pop(MessageType&), pop()
     */
    std::optional<MessageType> try_pop()
    {
        std::unique_lock lock{ mutex_ };

        if (queue_.empty())
            return std::nullopt;

        auto msg = std::optional<MessageType>{ std::move(queue_.front()) };
        queue_.pop_front();
        lock.unlock();
        cv_slot_available_.notify_one();
        return msg;
    }

    /**
     * If a message is available, remove it from the front of the queue and return it via
     * an output parameter with a function return value of true; otherwise, return false.
     *
     * \param msg  Output parameter to which the message is assigned if available; if the
     *             queue is empty, the value of this parameter is not changed.
     * \returns true if a message was successfully removed from the queue and assigned to
     *          the output parameter; false if the queue was empty.
     *
     * <h3>Exception Safety</h3>
     * If an exception occurs while moving the message, that exception is rethrown. The
     * message remains in the queue, but its value and the value of the output parameter
     * depend on the guarantees of the message type.
     *
     * \see try_pop(MessageType&), pop()
     */
    bool try_pop(MessageType& msg)
    {
        std::unique_lock lock{ mutex_ };

        if (queue_.empty())
            return false;

        msg = std::move(queue_.front());
        queue_.pop_front();
        lock.unlock();
        cv_slot_available_.notify_one();
        return true;
    }

    /**
     * Try to insert a message at the end of the queue.
     *
     * This call returns true if the message was successfully enqueued or false if the
     * queue temporarily had no space to store the message.
     *
     * Messages given as an rvalue are only moved from if they can actually be inserted
     * into the queue.
     *
     * <h3>Exception Safety</h3>
     * If the assignment of the message into the queue throws, the exception is rethrown
     * and no message is inserted.
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
        cv_message_available_.notify_all();
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
