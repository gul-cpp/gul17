/**
 * \file  thread_pool.cc
 * \date  Created on February 9, 2024
 * \brief Example on how to use the ThreadPool.
 *
 * \copyright Copyright 2024 Deutsches Elektronen-Synchrotron (DESY), Hamburg
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

#include <iostream>

#include <gul17/ThreadPool.h>
#include <gul17/time_util.h>

using namespace gul17;
using std::cout;

using namespace std::literals;

int main()
{
//! [Using the ThreadPool class]
    // Create a pool with 2 threads (returns a shared_ptr<ThreadPool>)
    auto pool = make_thread_pool(2);

    pool->add_task([]() { cout << "Task 1\n"; });
    pool->add_task([]() { sleep(1); std::cout << "Task 2\n"; });

    // Tasks can be scheduled to start later:
    // This one should start 2 seconds after enqueueing (if a thread is available)
    pool->add_task([]() { cout << "Task 3\n"; }, 2s);

    // Probable output:
    // Task 1
    // Task 2
    // Task 3

    // Tasks can return results
    auto task = pool->add_task([]() { return 42; });

    while (not task.is_complete())
        sleep(0.1);

    // get_result() blocks until the task is complete
    cout << "Task result: " << task.get_result() << "\n";

    // Tasks can also interact with the pool themselves, e.g. to schedule a continuation:
    pool->add_task(
        [](ThreadPool& pool) {
            cout << "Task 4\n";
            pool.add_task([]() { cout << "Task 5, a second later\n"; }, 1s);
        });
//! [Using the ThreadPool class]

    return 0;
}
