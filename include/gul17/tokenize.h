/**
 * \file    tokenize.h
 * \authors \ref contributors
 * \date    Created on September 3, 2018
 * \brief   Implementation of tokenize(), tokenize_sv().
 *
 * \copyright Copyright 2018-2024 Deutsches Elektronen-Synchrotron (DESY), Hamburg
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

#ifndef GUL17_TOKENIZE_H_
#define GUL17_TOKENIZE_H_

#include <string_view>
#include <string>
#include <vector>

#include "gul17/internal.h"
#include "gul17/string_util.h"

namespace gul17 {

/**
 * \addtogroup tokenize_h gul17/tokenize.h
 * \brief Split a string into substrings.
 * @{
 */

/**
 * Split the given string into a vector of substrings (tokens) delimited by any of the
 * characters in the delimiters string. Multiple adjacent delimiters are treated like a
 * single one, and delimiters at the beginning and end of the string are ignored.
 * For example, tokenize("   A  B C  ") yields a vector with the three entries "A", "B",
 * and "C".
 *
 * \code
 * // Default return type std::vector<std::string>
 * auto tokens = tokenize("  Hello world  ");
 * assert(tokens.size() == 2);
 * assert(tokens[0] == "Hello");
 * assert(tokens[1] == "world");
 * \endcode
 *
 * This function returns a `std::vector<std::string>` by default, but a compatible
 * container for string/string_view types can be specified via a template parameter:
 *
 * \tparam StringContainer     A container for strings or std::string_view-like types, e.g.
 *                             std::vector<std::string> or std::list<std::string_view>
 * \tparam ContainerInsertFct  Type for the \c insert_fct function parameter.
 *
 * \param str        The string to be split.
 * \param delimiters String with delimiter characters. Any of the characters in this
 *                   string marks the beginning/end of a token. By default, a wide
 *                   variety of whitespace and control characters is used.
 * \param insert_fct By default, tokenize() calls the \c emplace_back() member function on
 *                   the container to insert strings. This parameter may contain a
 *                   different function pointer or object with the signature
 *                   `void f(StringContainer&, std::string_view)` that is called
 *                   instead. This can be useful for containers that do not provide
 *                   \c emplace_back() or for other customizations.
 *
 * \returns a container with the single substrings.
 *
 * \code
 * // Return std::string_views instead of strings (like tokenize_sv())
 * auto parts1 = tokenize<std::vector<std::string_view>>("Hello world");
 * assert(parts1.size() == 2);
 * assert(parts1[0] == "Hello");
 * assert(parts1[1] == "world");
 *
 * // Use a different container that provides emplace_back()
 * auto parts2 = tokenize<gul17::SmallVector<std::string_view, 3>>("a-b-c", "-");
 * assert(parts2.size() == 3);
 * assert(parts2[0] == "a");
 * assert(parts2[1] == "b");
 * assert(parts2[2] == "c");
 *
 * // Use a different container with a custom inserter function
 * using WeirdContainer = std::queue<std::string>;
 * auto inserter = [](WeirdContainer& c, std::string_view sv) { c.emplace(sv); };
 * auto parts3 = tokenize<WeirdContainer>("a.b", ".", inserter);
 * assert(parts3.size() == 2);
 * assert(parts3.front() == "a");
 * assert(parts3.back() == "b");
 * \endcode
 *
 * \note
 * tokenize() does not assume a specific encoding for its input strings, but operates on
 * individual `char`s. This can have surprising effects in code such as this:
 * ```
 * auto words = tokenize("Hörgeräteakkustiker hätten es gewußt", "ä");
 * assert(words.size() == 3); // Might fail or succeed depending on the encoding
 * ```
 *
 * \see gul17::tokenize_sv() returns a vector<std::string_view> by default, gul17::split() uses
 *      a different approach to string splitting.
 *
 * \since GUL version 2.5, the return type of split() can be specified as a template
 *        parameter and a custom inserter can be specified (it always returned
 *        std::vector<std::string> before).
 */
template <typename StringContainer = std::vector<std::string>,
          typename ContainerInsertFct = void (*)(StringContainer&, std::string_view)>
inline StringContainer
tokenize(std::string_view str, std::string_view delimiters = default_whitespace_characters,
         ContainerInsertFct insert_fct = detail::emplace_back<StringContainer>)
{
    StringContainer tokens{ };

    std::string_view::size_type token_start = 0;
    std::string_view::size_type token_end = 0;

    while (true)
    {
        token_start = str.find_first_not_of(delimiters, token_end);
        if (token_start == std::string_view::npos)
            break;

        token_end = str.find_first_of(delimiters, token_start);
        if (token_end == std::string_view::npos)
        {
            insert_fct(tokens,
                std::string_view{ str.data() + token_start, str.length() - token_start });
            break;
        }
        else
        {
            insert_fct(tokens,
                std::string_view{ str.data() + token_start, token_end - token_start });
        }
    }

    return tokens;
}

/**
 * Split the given string into a vector of substrings (tokens) delimited by any of the
 * characters in the delimiters string.
 *
 * This function is identical to tokenize(std::string_view, std::string_view, ContainerInsertFct)
 * except that it returns a std::vector of std::string_views instead of strings by default:
 * \code
 * auto tokens = tokenize_sv("hello world", " "); // Return type is std::vector<std::string_view>
 * assert(tokens.size() == 2);
 * assert(tokens[0] == "hello");
 * assert(tokens[1] == "world");
 * \endcode
 *
 * \see gul17::tokenize() returns a vector<string> by default, gul17::split() uses a
 *      different approach to string splitting.
 *
 * \since GUL version 2.5, the return type of tokenize_sv() can be specified as a template
 *        parameter and a custom inserter can be specified (it always returned
 *        std::vector<std::string_view> before).
 */
template <typename StringContainer = std::vector<std::string_view>,
          typename ContainerInsertFct = void (*)(StringContainer&, std::string_view)>
inline StringContainer
tokenize_sv(std::string_view str, std::string_view delimiters = default_whitespace_characters,
            ContainerInsertFct insert_fct = detail::emplace_back<StringContainer>)
{
    return tokenize<StringContainer>(str, delimiters, insert_fct);
}

/// @}

} // namespace gul17

#endif
