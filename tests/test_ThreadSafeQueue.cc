/**
 * \file  test_ThreadSafeQueue.cc
 * \date  Created on April 1, 2022
 * \brief Test suite for the ThreadSafeQueue class template.
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

#include <thread>
#include <type_traits>

#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>

#include "gul17/ThreadSafeQueue.h"
#include "gul17/time_util.h"

using namespace gul17;

namespace {

struct Message
{
    int value_;

    Message(int v = 0)
        : value_{ v }
    {
    }
};

} // anonymous namespace

TEMPLATE_TEST_CASE("ThreadSafeQueue: Constructor", "[ThreadSafeQueue]", int, std::string,
    Message, std::unique_ptr<Message>)
{
    static_assert(std::is_constructible<ThreadSafeQueue<TestType>, uint32_t>::value,
        "ThreadSafeQueue<TestType> is constructible");

    ThreadSafeQueue<TestType> queue{ 4 };
}

TEST_CASE("ThreadSafeQueue: back()", "[ThreadSafeQueue]")
{
    ThreadSafeQueue<Message> queue{ 2 };

    queue.push(Message(1));
    auto msg_1 = queue.back();
    REQUIRE(queue.size() == 1);
    REQUIRE(msg_1.value_ == 1);

    queue.push(Message(2));
    REQUIRE(queue.size() == 2);
    auto msg_2 = queue.back();
    REQUIRE(queue.size() == 2);
    REQUIRE(msg_2.value_ == 2);

    auto msg_pop = queue.pop();
    REQUIRE(queue.size() == 1);
    REQUIRE(msg_pop.value_ == 1);
    auto msg_back = queue.back();
    REQUIRE(queue.size() == 1);
    REQUIRE(msg_back.value_ == 2);
}

TEST_CASE("ThreadSafeQueue: capacity()", "[ThreadSafeQueue]")
{
    ThreadSafeQueue<Message> queue(42);
    REQUIRE(queue.capacity() == 42u);
}

TEMPLATE_TEST_CASE("ThreadSafeQueue: empty()", "[ThreadSafeQueue]", int, std::string,
    Message, std::unique_ptr<Message>)
{
    ThreadSafeQueue<TestType> queue{ 10 };
    REQUIRE(queue.empty() == true);

    queue.push(TestType{});
    REQUIRE(queue.empty() == false);

    queue.pop();
    REQUIRE(queue.empty() == true);
}

TEMPLATE_TEST_CASE("ThreadSafeQueue: pop() single-threaded", "[ThreadSafeQueue]", int,
    std::string, Message, std::unique_ptr<Message>)
{
    ThreadSafeQueue<TestType> queue{ 10 };
    REQUIRE(queue.size() == 0);

    queue.push(TestType{});
    queue.push(TestType{});
    REQUIRE(queue.size() == 2u);

    queue.pop();
    REQUIRE(queue.size() == 1u);

    queue.pop();
    REQUIRE(queue.size() == 0);
}

TEST_CASE("ThreadSafeQueue: push() single-threaded", "[ThreadSafeQueue]")
{
    ThreadSafeQueue<Message> queue{ 10 };
    REQUIRE(queue.size() == 0);

    queue.push(Message(42));
    REQUIRE(queue.size() == 1u);

    queue.push(Message(43));
    REQUIRE(queue.size() == 2u);

    auto msg = queue.pop();
    REQUIRE(queue.size() == 1u);
    REQUIRE(msg.value_ == 42);

    msg = queue.pop();
    REQUIRE(queue.size() == 0);
    REQUIRE(msg.value_ == 43);
}

TEST_CASE("ThreadSafeQueue: push() & pop() across threads", "[ThreadSafeQueue]")
{
    // Create a queue with only 4 slots
    ThreadSafeQueue<Message> queue{ 4 };

    // Start a thread that will push 100 messages into the queue
    std::thread sender(
        [&queue]()
        {
            for (int i = 1; i <= 100; ++i)
                queue.push(Message(i));
        });

    gul17::sleep(0.005);
    // Pull all 100 messages out of the queue from the main thread
    for (int i = 1; i <= 100; ++i)
    {
        auto msg = queue.pop();
        REQUIRE(msg.value_ == i);
    }

    sender.join();
}

TEST_CASE("ThreadSafeQueue: size()", "[ThreadSafeQueue]")
{
    ThreadSafeQueue<Message> queue{ 10 };
    REQUIRE(queue.size() == 0);

    queue.push(Message{});
    REQUIRE(queue.size() == 1u);

    queue.push(Message{});
    REQUIRE(queue.size() == 2u);

    queue.pop();
    REQUIRE(queue.size() == 1u);

    queue.pop();
    REQUIRE(queue.size() == 0);
}

TEST_CASE("ThreadSafeQueue: try_pop() single-threaded", "[ThreadSafeQueue]")
{
    ThreadSafeQueue<int> queue{ 2 };
    REQUIRE(queue.size() == 0);

    REQUIRE(queue.try_pop() == std::nullopt);

    queue.push(1);
    queue.push(2);
    REQUIRE(queue.size() == 2u);

    auto opt = queue.try_pop();
    REQUIRE(opt.has_value());
    REQUIRE(*opt == 1);

    opt = queue.try_pop();
    REQUIRE(opt.has_value());
    REQUIRE(*opt == 2);

    REQUIRE(queue.try_pop() == std::nullopt);
}

TEST_CASE("ThreadSafeQueue: try_push() single-threaded", "[ThreadSafeQueue]")
{
    ThreadSafeQueue<int> queue{ 2 };
    REQUIRE(queue.size() == 0);

    REQUIRE(queue.try_push(1) == true);
    REQUIRE(queue.size() == 1u);

    REQUIRE(queue.try_push(2) == true);
    REQUIRE(queue.size() == 2u);

    REQUIRE(queue.try_push(3) == false); // queue full
    REQUIRE(queue.size() == 2u);

    int msg = queue.pop();
    REQUIRE(queue.size() == 1u);
    REQUIRE(msg == 1);

    REQUIRE(queue.try_push(3) == true);
    REQUIRE(queue.size() == 2u);

    msg = queue.pop();
    REQUIRE(queue.size() == 1u);
    REQUIRE(msg == 2);

    msg = queue.pop();
    REQUIRE(queue.empty() == true);
    REQUIRE(msg == 3);
}

TEST_CASE("ThreadSafeQueue: try_push() & try_pop() across threads", "[ThreadSafeQueue]")
{
    // Create a queue with only 4 slots
    ThreadSafeQueue<Message> queue{ 4 };

    // Start a thread that will push 100 messages into the queue
    std::thread sender(
        [&queue]()
        {
            for (int i = 1; i <= 100; ++i)
            {
                while (queue.try_push(Message(i)) == false)
                    ;
            }
        });

    // Pull all 100 messages out of the queue from the main thread
    for (int i = 1; i <= 100; ++i)
    {
        std::optional<Message> opt_msg;
        do
        {
            opt_msg = queue.try_pop();
        }
        while (not opt_msg.has_value());

        auto msg = *opt_msg;
        REQUIRE(msg.value_ == i);
    }

    sender.join();
}
