/**
 * \file   yaml_processor.cc
 * \author Jan Behrens
 * \date   Created on 20 November 2025
 * \brief  Implementation of the YAML data processor functions.
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
#include "gul17/join_split.h"

#include <algorithm>
#include <sstream>

using gul17::DataTree;

struct YamlDataParser
{
    YamlDataParser(const std::string_view& yaml_str)
        : data_(yaml_str)
    {}

    DataTree parse()
    {
        return parse_document();
    }

private:
    DataTree parse_document()
    {
        // Split into lines and reset state
        lines_.clear();
        current_line_ = 0;

        for (const auto & line : gul17::split_sv(data_, "\n"))
        {
            // Remove comments and skip empty lines
            auto stripped = strip_comment(line);
            if (!trim(stripped).empty())
            {
                lines_.emplace_back(stripped);
            }
        }

        if (lines_.empty())
            return DataTree(nullptr);

        return parse_node();
    }

    DataTree parse_node(size_t current_indent = 0)
    {
        if (current_line_ >= lines_.size())
            return DataTree(nullptr);

        auto line = lines_[current_line_];
        auto line_indent = get_indentation(line);
        auto content = trim(line.substr(line_indent));

        // Check if we're at the wrong indentation level
        if (line_indent < current_indent)
            return DataTree(nullptr); // Signal to go back

        // Determine node type
        if (is_sequence_item(content))
        {
            return parse_sequence(current_indent);
        }
        else if (is_mapping_item(content))
        {
            return parse_mapping(current_indent);
        }
        else
        {
            // Simple scalar value
            current_line_++;
            return parse_scalar(content);
        }
    }

    DataTree parse_sequence(size_t current_indent)
    {
        DataTree::Array sequence;

        while (current_line_ < lines_.size())
        {
            auto line = lines_[current_line_];
            auto line_indent = get_indentation(line);

            if (line_indent < current_indent)
                break; // End of this sequence

            auto content = trim(line.substr(line_indent));

            if (is_sequence_item(content)) // Starts with '-'
            {
                // Remove the sequence marker and parse the value
                auto item_content = trim(content.substr(1)); // Remove '-'
                current_line_++;

                // Check if this is a complex item (object or nested sequence)
                if (current_line_ < lines_.size())
                {
                    auto next_indent = get_indentation(lines_[current_line_]);
                    if (next_indent > line_indent)
                    {
                        // Nested structure
                        sequence.push_back(parse_node(next_indent));
                    }
                    else
                    {
                        // Simple scalar
                        sequence.push_back(parse_scalar(item_content));
                    }
                }
                else
                {
                    sequence.push_back(parse_scalar(item_content));
                }
            }
            else
            {
                break; // Not a sequence item anymore
            }
        }

        return DataTree(sequence);
    }

    DataTree parse_mapping(size_t current_indent)
    {
        DataTree::Object mapping;

        while (current_line_ < lines_.size())
        {
            auto line = lines_[current_line_];
            auto line_indent = get_indentation(line);

            if (line_indent < current_indent)
                break; // End of this mapping

            auto content = trim(line.substr(line_indent));

            if (is_mapping_item(content)) // Contains ':'
            {
                // Parse key-value pair
                auto colon_pos = content.find(':');
                auto key = trim(content.substr(0, colon_pos));
                auto value_str = trim(content.substr(colon_pos + 1));

                current_line_++;

                DataTree value;

                if (value_str.empty())
                {
                    // Value might be on next lines (complex value)
                    if (current_line_ < lines_.size())
                    {
                        auto next_indent = get_indentation(lines_[current_line_]);
                        if (next_indent > line_indent)
                        {
                            value = parse_node(next_indent);
                        }
                        else
                        {
                            value = DataTree(nullptr); // null for empty value
                        }
                    }
                    else
                    {
                        value = DataTree(nullptr); // null for empty value
                    }
                }
                else
                {
                    // Simple scalar value
                    value = parse_scalar(value_str);
                }

                mapping[std::string(key)] = value;
            }
            else
            {
                break; // Not a mapping item
            }
        }

        return DataTree(mapping);
    }

    DataTree parse_scalar(const std::string_view& value)
    {
        auto trimmed = trim(value);

        // Check for null
        if (trimmed == "null" || trimmed == "~" || trimmed.empty())
            return DataTree(nullptr);

        // Check for boolean
        if (trimmed == "true") return DataTree(true);
        if (trimmed == "false") return DataTree(false);

        // Check for number (integer)
        if (trimmed[0] == '-' || std::isdigit(trimmed[0]))
        {
            try
            {
                if (trimmed.find('.') == std::string::npos)
                {
                    size_t pos;
                    auto int_val = std::stoi(std::string(trimmed), &pos);
                    if (pos == trimmed.length())  // Entire string was converted
                    {
                        return DataTree(int_val);
                    }
                }
            }
            catch (...)
            {
                // Not an integer, try float
            }

            // Check for number (float)
            try
            {
                size_t pos;
                auto double_val = std::stod(std::string(trimmed), &pos);
                if (pos == trimmed.length())  // Entire string was converted
                {
                    return DataTree(double_val);
                }
            }
            catch (...)
            {
                // Not a number
            }
        }

        // Remove quotes if present and unescape
        if ((trimmed.front() == '"' && trimmed.back() == '"') ||
            (trimmed.front() == '\'' && trimmed.back() == '\''))
        {
            auto unquoted = trimmed.substr(1, trimmed.length() - 2);
            return DataTree(unescape_yaml_string(unquoted));
        }

        // Default to string
        return DataTree(std::string(trimmed));
    }

    size_t get_indentation(const std::string_view& line)
    {
        size_t i = 0;
        while (i < line.length() && (line[i] == ' ' || line[i] == '\t'))
        {
            i++;
        }

        return i;
    }

    std::string_view strip_comment(const std::string_view& line)
    {
        auto comment_pos = line.find('#');
        if (comment_pos != std::string::npos)
            return line.substr(0, comment_pos);

        return line;
    }

    std::string_view trim(const std::string_view& str)
    {
        auto start = str.find_first_not_of(" \t\n\r");
        if (start == std::string::npos)
            return "";

        auto end = str.find_last_not_of(" \t\n\r");
        return str.substr(start, end - start + 1);
    }

    bool is_sequence_item(const std::string_view& line)
    {
        auto trimmed = trim(line);
        return !trimmed.empty() && trimmed[0] == '-';
    }

    bool is_mapping_item(const std::string_view& line)
    {
        return line.find(':') != std::string::npos;
    }

    static std::string unescape_yaml_string(const std::string_view& str)
    {
        // TODO - Implement full YAML string unescaping

        std::string result;
        for (size_t i = 0; i < str.length(); ++i)
        {
            if (str[i] == '\\' && i + 1 < str.length())
            {
                switch (str[i + 1])
                {
                    case '"': result += '\"'; break;
                    case '\'': result += '\''; break;
                    case '\\': result += '\\'; break;
                    case '/': result += '/'; break;
                    case 'a': result += '\a'; break;
                    case 'b': result += '\b'; break;
                    case 'f': result += '\f'; break;
                    case 'n': result += '\n'; break;
                    case 'r': result += '\r'; break;
                    case 't': result += '\t'; break;
                    case 'v': result += '\v'; break;
                    case ' ': result += ' '; break;

                    // YAML-specific escapes
                    case '_': result += u8"\u00A0"; break;     // U+00A0 (non-breaking space)
                    case 'N': result += u8"\u0085"; break;     // U+0085 (next line)
                    case 'L': result += u8"\u2028"; break;     // U+2028 (line separator)
                    case 'P': result += u8"\u2029"; break;     // U+2029 (paragraph separator)

                    // Hexcode and Unicode escapes
                    case 'x':
                        if (i + 3 < str.length())
                        {
                            auto hex = str.substr(i + 2, 2);
                            try {
                                auto ch = std::stoi(std::string(hex), nullptr, 16);
                                result += static_cast<char>(ch);
                                i += 2;
                            }
                            catch (...) {
                                result += str[i + 1]; // Invalid hex, treat as literal
                            }
                        }
                        break;

                    case 'u':
                        // Unicode escape sequence (e.g., \uXXXX)
                        if (i + 5 < str.length())
                        {
                            auto num = str.substr(i + 2, 4);
                            try {
                                auto ch = std::stoi(std::string(num), nullptr, 16);
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
                                i += 4;
                            }
                            catch (...) {
                                result += str[i + 1]; // Invalid number, treat as literal
                            }
                        }
                        break;

                    case 'U':
                        // FIXME - Unicode escape sequences not implemented yet
                        throw std::runtime_error("Unicode escape sequences not supported");

                    default:
                        // Unknown escape - treat as literal character
                        result += str[i + 1];
                }
                ++i; // Skip next character after escape `\`
            }
            else
            {
                result += str[i];
            }
        }

        return result;
    }

private:
    std::string_view data_;
    std::vector<std::string_view> lines_;
    size_t current_line_{0};
};

struct YamlDataSerializer
{
    YamlDataSerializer(const DataTree& tree_root)
        : tree_root_(tree_root)
    {}

    std::string serialize(size_t indent)
    {
        if (indent == 0)
            throw std::runtime_error("Indentation must be greater than zero for YAML serialization");
        serialize_yaml(tree_root_, indent);
        return output_.str();
    }

private:
    void serialize_yaml(const DataTree& value, size_t indent, size_t current_indent = 0)
    {
        if (value.is_object())
        {
            serialize_mapping(value.as<DataTree::Object>(), indent, current_indent);
        }
        else if (value.is_array())
        {
            serialize_sequence(value.as<DataTree::Array>(), indent, current_indent);
        }
        else
        {
            serialize_scalar(value);
        }
    }

    void serialize_scalar(const DataTree& value)
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
            output_ << value.as<int>();
        }
        else if (value.is_double())
        {
            output_ << value.as<double>();
        }
        else if (value.is_string())
        {
            std::string str = value.as<std::string>();
            // Quote strings if they contain special characters
            if (str.empty() || str.find_first_of(":#{}[]&*!|>\"'%") != std::string::npos)
            {
                output_ << "\"" << str << "\"";
            }
            else
            {
                output_ << str;
            }
        }
    }

    void serialize_sequence(const DataTree::Array& arr, size_t indent, size_t current_indent)
    {
        for (const auto& item : arr)
        {
            output_ << std::string(current_indent, ' ') << "- ";
            if (item.is_object() || item.is_array())
            {
                output_ << "\n";
                serialize_yaml(item, indent, current_indent + indent);
            }
            else
            {
                serialize_scalar(item);
                output_ << "\n";
            }
        }
    }

    void serialize_mapping(const DataTree::Object& obj, size_t indent, size_t current_indent)
    {
        // Sort keys for consistent output
        std::vector<DataTree::Object::key_type> keys;
        std::transform(obj.begin(), obj.end(), std::back_inserter(keys),
                        [](const auto& pair) { return pair.first; });
        std::sort(keys.begin(), keys.end());

        for (size_t i = 0; i < keys.size(); ++i)
        {
            const auto& key = keys[i];
            const auto& val = obj.at(key);

            output_ << std::string(current_indent, ' ') << key << ":";

            if (val.is_object() || val.is_array())
            {
                output_ << "\n";
                serialize_yaml(val, indent, current_indent + indent);
            }
            else
            {
                output_ << " ";
                serialize_scalar(val);
                output_ << "\n";
            }
        }
    }

private:
    const DataTree& tree_root_;
    std::ostringstream output_;
};

namespace gul17 {

DataTree from_yaml_string(const std::string_view& data)
{
    YamlDataParser parser(data);
    return parser.parse();
}

std::string to_yaml_string(const DataTree& value, size_t indent)
{
    YamlDataSerializer serializer(value);
    return serializer.serialize(indent);
}

} // namespace gul17

// vi:ts=4:sw=4:sts=4:et
