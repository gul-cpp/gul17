/**
 * \file  escape.cc
 * \brief Implementation of escape(), unescape().
 * \date  Created on 31 August 2018
 *
 * \copyright Copyright 2018-2021 Deutsches Elektronen-Synchrotron (DESY), Hamburg
 *
 * Distributed under the Boost Software License, Version 1.0 (see \ref license_boost_1_0
 * or https://www.boost.org/LICENSE_1_0.txt).
 */

#include <limits>
#include <regex>

#include "gul17/escape.h"
#include "gul17/string_util.h"

using namespace std::literals::string_literals;

namespace gul17 {

std::string escape(std::string_view in)
{
    static_assert(std::numeric_limits<signed char>::min() == -128,
                  "Unsupported char type");
    static_assert(std::numeric_limits<signed char>::max() == 127,
                  "Unsupported char type");

    auto escaped = ""s;
    escaped.reserve(in.length());

    for (const signed char c : in)
    {
        switch (c)
        {
            case '"':
                escaped += "\\\"";
                break;
            case '\\':
                escaped += "\\\\";
                break;
            case '\n':
                escaped += "\\n";
                break;
            case '\r':
                escaped += "\\r";
                break;
            case '\t':
                escaped += "\\t";
                break;
            default:
                if (c < 32)
                {
                    // This applies also to all non-ASCII characters (>127) because they
                    // are mapped to negative values
                    escaped += "\\x";
                    escaped += hex_string(c);
                }
                else
                {
                    escaped += c;
                }
        }
    }

    return escaped;
}

std::string unescape(std::string_view in)
{
    auto const static re = std::regex{ R"(\\(["\\nrt]|x[[:xdigit:]]{2}))" };
    auto rit  = std::regex_iterator<std::string_view::const_iterator>{ in.cbegin(), in.cend(), re };
    auto const rend = std::regex_iterator<std::string_view::const_iterator>{ };
    auto last = decltype(*rit){ };
    auto unescaped = ""s;
    unescaped.reserve(in.length());

    if (rit == rend) {
        unescaped.assign(in.data(), in.size());
        return unescaped;
    }

    for (; rit != rend; ++rit) {
        last = *rit;
        unescaped += rit->prefix();
        if (rit->empty())
            continue;
        auto const matched_escape_sequence = rit->str(1);
        switch (matched_escape_sequence[0]) {
        case '\"':
        case '\\':
            unescaped += matched_escape_sequence;
            break;
        case 'n':
            unescaped += "\n";
            break;
        case 'r':
            unescaped += "\r";
            break;
        case 't':
            unescaped += "\t";
            break;
        case 'x':
            unescaped += static_cast<char>(std::stoi(matched_escape_sequence.substr(1), 0, 16));
            break;
        default:
            unescaped += rit->str();
            break;
        }
    }

    unescaped += last.suffix();

    return unescaped;
}

} // namespace gul17

// vi:ts=4:sw=4:sts=4:et
