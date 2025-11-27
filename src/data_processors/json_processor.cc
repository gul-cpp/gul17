/**
 * \file   json_processor.cc
 * \author Jan Behrens
 * \date   Created on 20 November 2025
 * \brief  Implementation of the JSON data processor functions.
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

#include "gul17/data_processors.h"
#include "gul17/cat.h"

#include <algorithm>
#include <sstream>

using gul17::DataTree;

struct JsonDataParser
{
    JsonDataParser(const std::string_view& json_str)
        : data_(json_str)
    {}

    DataTree parse()
    {
        return parse_value();
    }

private:
    DataTree parse_value()
    {
        skip_comment();
        skip_whitespace();
        char c = current_char();

        switch (c)
        {
            case '{': return parse_object();
            case '[': return parse_array();
            case '"': return parse_string();
            case 't': case 'f': return parse_boolean();
            case 'n': return parse_null();
            case '-':
            case '0': case '1': case '2': case '3': case '4':
            case '5': case '6': case '7': case '8': case '9':
                return parse_number();

            default:
                throw std::runtime_error(gul17::cat("Unexpected character: ", c, " at position ", pos_));
        }
    }

    DataTree parse_object()
    {
        expect('{');
        DataTree::Object obj;

        skip_comment();
        skip_whitespace();
        if (current_char() == '}')
        {
            advance();
            return DataTree(obj);
        }

        while (true)
        {
            skip_whitespace();
            std::string key = parse_string().as<std::string>();

            skip_whitespace();
            expect(':');

            DataTree value = parse_value();
            obj.emplace(std::move(key), std::move(value));

            skip_whitespace();
            if (current_char() == '}')
            {
                advance();
                break;
            }
            expect(',');
        }

        return DataTree(obj);
    }

    DataTree parse_array()
    {
        expect('[');
        DataTree::Array arr;

        skip_comment();
        skip_whitespace();
        if (current_char() == ']')
        {
            advance();
            return DataTree(arr);
        }

        while (true)
        {
            arr.push_back(parse_value());

            skip_whitespace();
            if (current_char() == ']')
            {
                advance();
                break;
            }
            expect(',');
        }

        return DataTree(arr);
    }

    DataTree parse_string()
    {
        expect('"');
        std::string result;

        while (true)
        {
            char c = current_char();
            if (c == '"')
            {
                advance();
                break;
            }
            else if (c == '\\')
            {
                // TODO - Implement full JSON string unescaping

                advance();
                char esc = current_char();
                switch (esc)
                {
                    case '"': result += '"'; break;
                    case '\\': result += '\\'; break;
                    case '/': result += '/'; break;
                    case 'b': result += '\b'; break;
                    case 'f': result += '\f'; break;
                    case 'n': result += '\n'; break;
                    case 'r': result += '\r'; break;
                    case 't': result += '\t'; break;

                    case 'u':
                        // Unicode escape sequence (e.g., \uXXXX)
                        if (pos_ + 5 < data_.length())
                        {
                            auto num = data_.substr(pos_ + 1, 4);
                            try {
                                unsigned ch = std::stoi(std::string(num), nullptr, 16);
                                if (ch < 0x80)
                                {
                                    result += static_cast<char>(ch);
                                }
                                else if (ch < 0x800)
                                {
                                    result += static_cast<char>(0xC0 | (ch >> 6));
                                    result += static_cast<char>(0x80 | (ch & 0x3F));
                                }
                                else if (ch < 0x10000)
                                {
                                    result += static_cast<char>(0xE0 | (ch >> 12));
                                    result += static_cast<char>(0x80 | ((ch >> 6) & 0x3F));
                                    result += static_cast<char>(0x80 | (ch & 0x3F));
                                }
                                else
                                {
                                    result += static_cast<char>(0xF0 | (ch >> 18));
                                    result += static_cast<char>(0x80 | ((ch >> 12) & 0x3F));
                                    result += static_cast<char>(0x80 | ((ch >> 6) & 0x3F));
                                    result += static_cast<char>(0x80 | (ch & 0x3F));
                                }
                                pos_ += 4;
                            }
                            catch (...) {
                                result += data_[pos_ + 1]; // Invalid number, treat as literal
                                pos_ += 1;
                            }
                        }
                        break;

                    case 'U':
                        // Unicode escape sequence (e.g., \UXXXXXXXX)
                        // FIXME - Unicode escape sequences not implemented yet
                        throw std::runtime_error("Unicode escape sequences not supported");

                    default:
                        throw std::runtime_error(gul17::cat("Invalid escape sequence: ", esc, " at position ", pos_));
                }
                advance();
            }
            else
            {
                result += c;
                advance();
            }
        }

        return DataTree(result);
    }

    DataTree parse_boolean()
    {
        if (data_.compare(pos_, 4, "true") == 0)
        {
            pos_ += 4;
            return DataTree(true);
        }
        else if (data_.compare(pos_, 5, "false") == 0)
        {
            pos_ += 5;
            return DataTree(false);
        }
        else
        {
            throw std::runtime_error(gul17::cat("Invalid boolean value at position ", pos_));
        }
    }

    DataTree parse_null()
    {
        if (data_.compare(pos_, 4, "null") == 0)
        {
            pos_ += 4;
            return DataTree(nullptr);
        }
        else
        {
            throw std::runtime_error(gul17::cat("Invalid null value at position ", pos_));
        }
    }

    DataTree parse_number()
    {
        auto start_pos = pos_;
        if (current_char() == '-')
        {
            advance();
        }

        while (std::isdigit(current_char()))
        {
            advance();
        }

        if (current_char() == '.')
        {
            advance();
            while (std::isdigit(current_char()))
            {
                advance();
            }
            double value = std::stod(std::string(data_.substr(start_pos, pos_ - start_pos)));
            return DataTree(value);
        }
        else
        {
            int value = std::stoi(std::string(data_.substr(start_pos, pos_ - start_pos)));
            return DataTree(value);
        }
    }

    void skip_whitespace()
    {
        while (pos_ < data_.size() && std::isspace(data_[pos_]))
        {
            advance();
        }
    }

    void skip_comment()
    {
        skip_whitespace();

        if (current_char() == '/')
        {
            // Skip comments
            if (next_char() == '/')
            {
                // Single-line comment
                while (has_remaining_chars() && current_char() != '\n')
                {
                    advance();
                }
            }
            else if (next_char() == '*')
            {
                // Multi-line comment
                advance(2);
                while (has_remaining_chars())
                {
                    if (current_char() == '*' && next_char() == '/')
                    {
                        advance(2);
                        break;
                    }
                    advance();
                }
            }
            else
            {
                throw std::runtime_error(gul17::cat("Invalid comment syntax at position ", pos_));
            }
        }
    }

    char current_char() const
    {
        return pos_ < data_.size() ? data_[pos_] : '\0';
    }

    char next_char() const
    {
        return pos_ + 1 < data_.size() ? data_[pos_+1] : '\0';
    }

    bool has_remaining_chars() const
    {
        return pos_ < data_.size();
    }

    void advance(size_t n = 1)
    {
        pos_ += n;
    }

    void expect(char expected)
    {
        if (current_char() != expected)
        {
            throw std::runtime_error(gul17::cat("Expected character not found: ", expected, " at position ", pos_));
        }
        advance();
    }

private:
    std::string_view data_;
    size_t pos_{0};
};

struct JsonDataSerializer
{
    JsonDataSerializer(const DataTree& tree_root)
        : tree_root_(tree_root)
    {}

    std::string serialize(size_t indent)
    {
        serialize_value(tree_root_, indent);
        return output_.str();
    }

private:
    void serialize_value(const DataTree& value, size_t indent, size_t current_indent = 0)
    {
        if (value.is_null())
        {
            output_ << "null";
        }
        else if (value.is_boolean())
        {
            output_ << (value.as<bool>() ? "true" : "false");
        }
        else if (value.is_int())
        {
            output_ << std::to_string(value.as<int>());
        }
        else if (value.is_double())
        {
            output_ << std::to_string(value.as<double>());
        }
        else if (value.is_string())
        {
            output_ << "\"" << escape_string(value.as<std::string>()) << "\"";
        }
        else if (value.is_array())
        {
            serialize_array(value.as<DataTree::Array>(), indent, current_indent);
        }
        else if (value.is_object())
        {
            serialize_object(value.as<DataTree::Object>(), indent, current_indent);
        }
    }

    void serialize_array(const DataTree::Array& arr, size_t indent, size_t current_indent)
    {
        output_ << "[";
        if (!arr.empty())
        {
            output_ << "\n";
            for (size_t i = 0; i < arr.size(); ++i)
            {
                output_ << std::string(current_indent + indent, ' ');
                serialize_value(arr[i], indent, current_indent + indent);

                if (i < arr.size() - 1)
                    output_ << ",";
                output_ << "\n";
            }
            output_ << std::string(current_indent, ' ');
        }
        output_ << "]";
    }

    void serialize_object(const DataTree::Object& obj, size_t indent, size_t current_indent)
    {
        output_ << "{";
        if (!obj.empty())
        {
            // Sort keys for consistent output
            std::vector<DataTree::Object::key_type> keys;
            std::transform(obj.begin(), obj.end(), std::back_inserter(keys),
                           [](const auto& pair) { return pair.first; });
            std::sort(keys.begin(), keys.end());

            output_ << "\n";
            for (size_t i = 0; i < keys.size(); ++i)
            {
                const auto& key = keys[i];
                const auto& val = obj.at(key);

                output_ << std::string(current_indent + indent, ' ');
                output_ << "\"" << escape_string(key) << "\": ";
                serialize_value(val, indent, current_indent + indent);

                if (i < keys.size() - 1)
                    output_ << ",";
                output_ << "\n";
            }
            output_ << std::string(current_indent, ' ');
        }
        output_ << "}";
    }

    static std::string escape_string(const std::string& str)
    {
        std::string result;
        result.reserve(str.size() + 2); // Reserve space for efficiency
        for (char c : str)
        {
            switch (c)
            {
                case '"': result += "\\\""; break;
                case '\\': result += "\\\\"; break;
                case '\a': result += "\\a"; break;
                case '\b': result += "\\b"; break;
                case '\f': result += "\\f"; break;
                case '\n': result += "\\n"; break;
                case '\r': result += "\\r"; break;
                case '\t': result += "\\t"; break;
                case '\v': result += "\\v"; break;

                default:
                    // escape control characters
                    if (static_cast<unsigned char>(c) < 0x20)
                    {
                        char buf[7];
                        snprintf(buf, sizeof(buf), "\\u%04x", static_cast<unsigned char>(c));
                        result += buf;
                    }
                    else
                    {
                        result += c;
                    }
            }
        }

        return result;
    }

private:
    const DataTree& tree_root_;
    std::ostringstream output_;
};

namespace gul17 {

DataTree from_json_string(const std::string_view& data)
{
    JsonDataParser parser(data);
    return parser.parse();
}

std::string to_json_string(const DataTree& value, size_t indent)
{
    JsonDataSerializer serializer(value);
    return serializer.serialize(indent);
}

} // namespace gul17

// vi:ts=4:sw=4:sts=4:et
