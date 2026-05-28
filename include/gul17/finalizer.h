/**
 * \file  finalizer.h
 * \brief Implementation of FinalAction and finally().
 * \date  Created on 23 October 2018
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

#ifndef GUL17_FINALIZER_H_
#define GUL17_FINALIZER_H_

#include <type_traits>
#include <utility>

#include "gul17/internal.h"

namespace gul17 {

/**
 * \addtogroup finalizer_h gul17/finalizer.h
 * \brief Executing some clean-up code when leaving a scope.
 * @{
 */

/**
 * FinalAction allows us to execute something if the FinalAction object leaves the scope.
 *
 * A FinalAction can be used to add RAII like behavior for non RAII object or to do
 * timing measurements.
 *
 * To generate this object you can use the function \ref finally, that leverages
 * template argument deduction of the action's type. This simplifies instantiation a lot.
 *
 * (Implementation is quite similar to what's in the Guideline-Support-Library.)
 *
 * A good example is function call duration measurement. The FinalAction just needs to
 * be created in the beginning of a function and no hassle with diverse possible return
 * points.
 * \code
 * #include <cstdlib>
 * #include <ctime>
 * #include <gul17/gul.h>
 *
 * using gul17::finally;
 * using gul17::sleep;
 * using gul17::tic;
 * using gul17::toc;
 *
 * void foo() {
 *     auto _ = finally([start = tic()] {
 *         std::cerr << "Function foo() took " << toc(start) << " s.\n";
 *     });
 *
 *     std::srand(std::time(nullptr));
 *     if (std::rand() % 2) {
 *         std::cout << "Premature exit\n";
 *         return;
 *     }
 *     sleep(10); // do important stuff (like sleeping)
 *     std::cout << "Normal exit\n";
 * }
 * \endcode
 *
 * A (bad; use containers instead) example is allocation with RAII:
 * \code
 * #include <new>
 * #include <string>
 * #include <gul17/gul.h>
 *
 * using gul17::finally;
 *
 * std::string bar(float some_float) {
 *     char* buffer = new char[100];
 *     if (buffer == nullptr)
 *         return;
 *     auto _ = finally([&] { delete[] buffer; buffer = nullptr; });
 *
 *     // do stuff that might throw here
 *
 *     snprintf(buffer, 100, "%.1f", some_float);
 *     return { buffer };
 *     // get rid of buffer automagically
 * }
 * \endcode
 *
 * \tparam F The type of the closure/function to be called.
 *
 * \since GUL version 1.1
 */
template <typename F>
class FinalAction
{
    F action_;
    bool invoke_ {true};

public:
    /**
     * Creates a new FinalAction object
     *
     * It takes any callable as action to be called when the FinalAction destructs
     * (lifetime ends / leaves the scope).
     *
     * The template parameter \b F has to be specified; this can be avoided by using
     * the convenience function \ref finally.
     *
     * \tparam F The type of the closure/function to be called.
     * \param f The closure or function to be called on destruction.
     */
    explicit FinalAction(F f) noexcept
        : action_(std::move(f))
    {}
    /// Move constructor
    FinalAction(FinalAction&& other) noexcept
        : action_{ std::move(other.action_) }
        , invoke_{ other.invoke_ }
    {
        // don't call callback on moved from FinalAction
        other.invoke_ = false;
    }
    /// Move assignment operator
    FinalAction& operator=(FinalAction&& other) noexcept {
        if (this != &other) {
            this->action_ = std::move(other.action_);
            this->invoke_ = std::move(other.invoke_);
            // don't call callback on moved from FinalAction
            other.invoke_ = false;
        }
        return *this;
    }

    FinalAction() = delete; ///< FinalAction is not is_default_constructible
    FinalAction(const FinalAction&) = delete; ///< FinalAction is not copyable
    FinalAction& operator=(const FinalAction&) = delete; ///< FinalAction is not copyable

    /// Destructor
    /// Calls \b action except when in move contexts.
    ~FinalAction() noexcept {
        if (invoke_)
            action_();
    }
};

/**
 * finally() - convenience function to generate a FinalAction
 *
 * A FinalAction can be used to add RAII like behavior for non RAII object or to do
 * timing measurements. More information given in the FinalAction documentation.
 *
 * \tparam F   The type of the closure/function to be called (normally autodeduced).
 * \param f    The closure or pointer to function to be called on destruction.
 *
 * \since GUL version 1.1
 */
template <typename F>
FinalAction<typename std::decay_t<F>> finally(F&& f) noexcept {
    return FinalAction<typename std::decay_t<F>>(std::forward<typename std::decay_t<F>>(f));
}

/// @}

} // namespace gul17

#endif

// vi:et:sts=4:sw=4:ts=4
