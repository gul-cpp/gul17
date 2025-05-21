/**
 * \file    join_split.h
 * \brief   Implementation of join(), split(), and split_sv().
 * \authors \ref contributors
 * \date    Created on 31 August 2018
 *
 * \copyright Copyright 2018-2025 Deutsches Elektronen-Synchrotron (DESY), Hamburg
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 2.1 of the license, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef GUL17_JOIN_SPLIT_H_
#define GUL17_JOIN_SPLIT_H_

#include <iterator>
#include <regex>
#include <string_view>
#include <string>
#include <type_traits>
#include <vector>

#include "gul17/internal.h"
#include "gul17/string_util.h"

namespace gul17 {

/**
 * \addtogroup join_split_h gul17/join_split.h
 * \brief Joining and splitting strings.
 * @{
 */

/**
 * Separate a string at all occurrences of a delimiter, returning the strings between the
 * delimiters in a container.
 *
 * The result has at least one element. If the delimiter is not present in the text, the
 * whole text is returned. If there are consecutive delimiters, the collected string
 * between them is the empty string. If the delimiter is directly at the end of the
 * input, the collected string between the end of the input and the delimiter is again the
 * empty string.
 *
 * split() is the inverse function of join(). It is guaranteed that
 * `join(split(text, del), del) == text`.
 *
 * \code
 * std::vector<std::string> parts1 = split(" hello world", " ");
 * assert(parts1.size() == 3);
 * assert(parts1[0] == ""s);
 * assert(parts1[1] == "hello"s);
 * assert(parts1[2] == "world"s);
 *
 * std::vector<std::string> parts2 = split("<>", "<>");
 * assert(parts2.size() == 2);
 * assert(parts2[0] == ""s);
 * assert(parts2[1] == ""s);
 * \endcode
 *
 * This function returns a `std::vector<std::string>` by default, but a compatible
 * container for string/string_view types can be specified via a template parameter:
 *
 * \tparam StringContainer     A container for strings or std::string_view-like types, e.g.
 *                             std::vector<std::string> or std::list<std::string_view>
 * \tparam ContainerInsertFct  Type for the \c insert_fct function parameter.
 *
 * \param text       The string to be split
 * \param delimiter  The delimiting substring
 * \param insert_fct By default, split() calls the \c emplace_back() member function on
 *                   the container to insert strings. This parameter may contain a
 *                   different function pointer or object with the signature
 *                   `void f(StringContainer&, std::string_view)` that is called
 *                   instead. This can be useful for containers that do not provide
 *                   \c emplace_back() or for other customizations.
 *
 * \returns a container filled with the substrings that were separated by delimiter in the
 *          original string.
 *
 * \code
 * // Different string type
 * auto parts1 = split<std::vector<std::string_view>>("Hello world", " "); // behaves like split_sv()
 * assert(parts1.size() == 2);
 * assert(parts1[0] == "Hello"); // No lifetime problems because "Hello world" is a
 * assert(parts1[1] == "world"); // string literal with static storage duration
 *
 * // Custom container type
 * auto parts2 = split<gul17::SmallVector<std::string, 3>>("a--b--c", "--");
 * assert(parts2.size() == 3);
 * assert(parts2[0] == "a");
 * assert(parts2[1] == "b");
 * assert(parts2[2] == "c");
 *
 * // For a container without emplace_back(), use a custom inserter:
 * using WeirdContainer = std::queue<std::string>;
 * auto inserter = [](WeirdContainer& c, std::string_view sv) { c.emplace(sv); };
 * auto parts3 = split<WeirdContainer>("a.b", ".", inserter);
 * assert(parts3.size() == 2);
 * assert(parts3.front() == "a");
 * assert(parts3.back() == "b");
 * \endcode
 *
 * \see split_sv() returns a vector of std::string_views,<br>
 *      split(const std::string&, const std::regex&) splits at a delimiter described by
 *      a regular expression, and<br>
 *      join() can join the vector back into a string.
 *
 * \since GUL version 2.5, the return type of split() can be specified as a template
 *        parameter and a custom inserter can be specified (it always returned
 *        std::vector<std::string> before).
 */
template <typename StringContainer = std::vector<std::string>,
          typename ContainerInsertFct = void (*)(StringContainer&, std::string_view)>
inline StringContainer
split(std::string_view text, std::string_view delimiter,
      ContainerInsertFct insert_fct = detail::emplace_back<StringContainer>)
{
    using StringType = typename StringContainer::value_type;
    using SizeType = typename StringType::size_type;

    auto result = StringContainer{ };
    auto search_start = SizeType{ 0 };
    auto push_start = search_start;

    for (;;) {
        const auto hit = text.find(delimiter.data(), search_start, delimiter.size());
        if (hit == StringType::npos)
            break;
        const auto hit_len = hit - push_start;
        insert_fct(result, text.substr(push_start, hit_len));
        search_start += std::max(delimiter.size() + hit_len, SizeType{ 1 });
        push_start += delimiter.size() + hit_len;
    }
    insert_fct(result, text.substr(push_start));
    return result;
}

/**
 * Separate a string at all occurrences of a delimiter described by a regular expression,
 * returning the strings between the delimiters in a container.
 *
 * This function is a variant of
 * split(std::string_view, std::string_view, ContainerInsertFct) that accepts a std::regex
 * object to describe the delimiter:
 * \code
 * // Return type is std::vector<std::string>
 * auto parts = split("one\ntwo\nthree"s, std::regex{"[^[:print:]]"});
 * assert(y.size() == 3);
 * assert(y[0] == "one"s);
 * assert(y[1] == "two"s);
 * assert(y[2] == "three"s);
 * \endcode
 *
 * \param text       The string to be split
 * \param delimiter  A std::regex object describing the delimiters
 * \param insert_fct Custom container inserter function
 *
 * \see
 * split(std::string_view, std::string_view, ContainerInsertFct) splits at a fixed substring,<br>
 * split_sv() does the same returning a vector of std::string_views, and<br>
 * join() can join the vector back into a string.
 */
template <typename StringContainer = std::vector<std::string>,
          typename ContainerInsertFct = void (*)(StringContainer&, std::string_view)>
inline StringContainer
split(std::string_view text, const std::regex& delimiter,
      ContainerInsertFct insert_fct = detail::emplace_back<StringContainer>)
{
    auto const end = std::cregex_iterator{ };
    auto result = StringContainer{ };

    auto parts = std::cregex_iterator(text.data(), text.data() + text.size(), delimiter);
    if (parts == end)
    {
        insert_fct(result, text);
    }
    else
    {
        auto previous = std::cregex_iterator{ };
        for (; parts != end; ++parts) {
            if (parts == previous and not parts->length())
                break;
            auto const& match = parts->prefix();
            insert_fct(result, std::string_view(match.first, match.length()));
            previous = parts;
        }

        auto const& match = previous->suffix();
        insert_fct(result, std::string_view(match.first, match.length()));
    }

    return result;
}

/**
 * Separate a string at all occurrences of a delimiter, returning the strings between the
 * delimiters in a vector.
 *
 * This function is identical to split(std::string_view, std::string_view, ContainerInsertFct)
 * except that it returns a std::vector of std::string_views instead of strings by default:
 * \code
 * auto parts = split_sv("hello world", " "); // Return type is std::vector<std::string_view>
 * assert(parts.size() == 2);
 * assert(parts[0] == "hello"); // No problems with lifetime because "hello world"
 * assert(parts[1] == "world"); // is a string literal with static storage duration
 * \endcode
 *
 * \see split(std::string_view, std::string_view, ContainerInsertFct) returns a vector of strings,<br>
 *      split(const std::string&, const std::regex&) splits at a delimiter described by
 *      a regular expression, and<br>
 *      join() can join the vector back into a string.
 *
 * \since GUL version 2.5, the return type of split_sv() can be specified as a template
 *        parameter and a custom inserter can be specified (it always returned
 *        std::vector<std::string> before).
 */
template <typename StringContainer = std::vector<std::string_view>,
          typename ContainerInsertFct = void (*)(StringContainer&, std::string_view)>
inline StringContainer
split_sv(std::string_view text, std::string_view delimiter,
      ContainerInsertFct insert_fct = detail::emplace_back<StringContainer>)
{
    return split<StringContainer>(text, delimiter, insert_fct);
}

namespace detail {

// Type trait to check if a type has a member function size()
template <typename, typename = void>
struct HasSize : std::false_type
{};

template <typename T>
struct HasSize<T, std::void_t<decltype(std::declval<const T&>().size())>> : std::true_type
{};

} // namespace detail

/**
 * Concatenate all strings (or string-like elements) in a range, placing a delimiter
 * between them.
 *
 * \code{.cpp}
 * std::vector<std::string> parts = { "one", "two", "three" };
 * std::string result = join(parts.begin(), parts.end(), "-");
 * assert(result == "one-two-three");
 * \endcode
 *
 * If the element type in the range has a size() member function, the algorithm iterates
 * twice over the range in order to pre-allocate a string of the correct size. Otherwise,
 * it only iterates once.
 *
 * \param begin  Iterator pointing to the first string
 * \param end    Iterator pointing past the last string
 * \param glue   String that is put between each element of parts
 *
 * \returns all strings glued together with the delimiter glue.
 *
 * \tparam Iterator  A forward iterator type that dereferences to a string-like type. This
 *                   string type must must support concatenation using
 *                   std::string::operator+=. An additional optimization is enabled if
 *                   the string type has a size() member function.
 *
 * \see
 * join(StringContainer, std::string_view) is a convenience overload for joining entire
 * containers. join(Container, std::string_view, ConversionFct) and join(Iterator,
 * Iterator, std::string_view, ConversionFct) allow joining ranges of arbitrary type into
 * a string using a conversion function. split() and associated functions can be used to
 * split a string into a vector of substrings.
 *
 * \since GUL version 2.3
 */
template <typename Iterator>
inline std::string
join(Iterator begin, Iterator end, std::string_view glue)
{
    using ElementType = decltype(*begin);
    std::string result;

    if (begin == end)
        return result; // Return an empty string

    if constexpr (detail::HasSize<ElementType>::value)
    {
        std::size_t num_strings = 0;
        std::size_t len = 0;

        for (auto it = begin; it != end; ++it)
        {
            ++num_strings;
            len += it->size();
        }
        len += (num_strings - 1) * glue.size();

        result.reserve(len);
    }

    result += *begin;

    // Iterate over all but the first string
    for (auto it = std::next(begin); it != end; ++it)
    {
        result.append(glue.data(), glue.size());
        result += *it;
    }

    return result;
}

/**
 * Concatenate all strings (or string-like elements) in a range, placing a delimiter
 * between them.
 *
 * \code{.cpp}
 * std::vector<std::string> parts = { "one", "two", "three" };
 * std::string result = join(parts, "-");
 * assert(result == "one-two-three");
 * \endcode
 *
 * If the element type in the range has a size() member function, the algorithm iterates
 * twice over the range in order to pre-allocate a string of the correct size. Otherwise,
 * it only iterates once.
 *
 * This is the inverse function of split(). It is guaranteed that
 * `join(split(text, del), del) == text` (unless del is a std::regex object).
 *
 * \param parts  A container holding strings or string views that are to be concatenated
 * \param glue   String that is put between each element of parts
 *
 * \returns all strings glued together with the delimiter glue.
 *
 * \tparam StringContainer  A container type that holds string-like elements, e.g.
 *                          std::vector<std::string> or std::list<std::string_view>.
 *                          The container must provide an STL-like forward iterator
 *                          interface. The string type must must support concatenation
 *                          using std::string::operator+=. An additional optimization is
 *                          enabled if the string type has a size() member function.
 *
 * \see
 * join(Iterator, Iterator, std::string_view) has a two-iterator interface.
 * join(Container, std::string_view, ConversionFct) and join(Iterator, Iterator,
 * std::string_view, ConversionFct) allow joining ranges of arbitrary type into
 * a string using a conversion function. split() and associated functions can be used to
 * split a string into a vector of substrings.
 *
 * \since GUL version 2.3, join() accepts arbitrary containers or iterators (it was
 *        limited to std::vector before).
 */
template <typename StringContainer>
inline std::string
join(const StringContainer& parts, std::string_view glue)
{
    return join(parts.begin(), parts.end(), glue);
}

/**
 * Concatenate the strings resulting from calling the given function on each element in a
 * range, placing a delimiter between them.
 *
 * \code{.cpp}
 * std::vector<int> parts = { 1, 2, 3 };
 * std::string result = join(parts.begin(), parts.end(), "-",
 *                           [](int i) { return std::to_string(i); });
 * assert(result == "1-2-3");
 * \endcode
 *
 * This algorithm iterates exactly once over the range. To speed up complex joining
 * operations, string memory can be preallocated via the last parameter.
 *
 * \param begin      Iterator to the first input element
 * \param end        Iterator pointing past the last input element
 * \param glue       String to be put between the string-converted elements
 * \param to_string  Function object to convert the container elements to strings
 * \param prealloc   Number of bytes to preallocate for the result string. Providing this
 *                   parameter is not necessary, but it may provide a tiny performance
 *                   boost. The default is zero (no preallocation).
 *
 * \returns the concatenated string.
 *
 * \tparam Iterator  A forward iterator type that dereferences to a type that can be
 *                   passed to the ConversionFct.
 * \tparam ConversionFct  A function object type that can be called with the dereferenced
 *                   iterator type and returns a string type. This string type must
 *                   support concatenation with std::string::operator+=().
 *
 * \since GUL version UNRELEASED
 */
template <typename Iterator, typename ConversionFct,
    std::enable_if_t<std::is_invocable_v<ConversionFct, decltype(*Iterator{})>, bool>
    = true>
inline std::string
join(Iterator begin, Iterator end, std::string_view glue, ConversionFct to_string,
     std::size_t prealloc = 0)
{
    std::string result;

    if (begin == end)
        return result; // Return an empty string

    result.reserve(prealloc);
    result += to_string(*begin);

    // Iterate over all but the first element of the range
    for (auto it = std::next(begin); it != end; ++it)
    {
        result += glue;
        result += to_string(*it);
    }

    return result;
}

/**
 * Concatenate the strings resulting from calling the given function on each element
 * in a range, placing a delimiter between them.
 *
 * \code{.cpp}
 * std::vector<int> parts = { 1, 2, 3 };
 * std::string result = join(parts, "-", [](int i) { return std::to_string(i); });
 * assert(result == "1-2-3");
 * \endcode
 *
 * This algorithm iterates exactly once over the range. To speed up complex joining
 * operations, string memory can be preallocated via the last parameter.
 *
 * \param container  Container of input elements
 * \param glue       String to be put between the string-converted elements
 * \param to_string  Function object to convert the container elements to strings
 * \param prealloc   Number of bytes to preallocate for the result string. Providing this
 *                   parameter is not necessary, but it may provide a tiny performance
 *                   boost. The default is zero (no preallocation).
 *
 * \returns the concatenated string.
 *
 * \tparam Container   A container type that provides a cbegin() and cend() member
 *                     function returning forward iterators.
 * \tparam ConversionFct  A function object type that can be called with the dereferenced
 *                     iterator type and returns a string type. This string type must
 *                     support concatenation with std::string::operator+=().
 *
 * \since GUL version UNRELEASED
 */
template <typename Container, typename ConversionFct,
    std::enable_if_t<std::is_invocable_v<ConversionFct,
                     decltype(*(std::declval<Container>().cbegin()))>, bool> = true>
inline std::string
join(const Container& container, std::string_view glue, ConversionFct to_string,
     std::size_t prealloc = 0)
{
    return join(container.cbegin(), container.cend(), glue, to_string, prealloc);
}

/// @}

} // namespace gul17

#endif

/* vim:set noexpandtab softtabstop=4 tabstop=4 shiftwidth=4 textwidth=90 cindent: */
