/**
 * \file   xml_processor.cc
 * \author Jan Behrens
 * \date   Created on 20 November 2025
 * \brief  Implementation of the XML data processor functions.
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
#include <unordered_map>
#include <vector>

using gul17::DataTree;

struct XmlDataParser
{
    XmlDataParser(const std::string_view& xml_str)
        : data_(xml_str)
    {}

    DataTree parse()
    {
        return parse_xml_element().second;
    }

private:
    using KeyValuePair = std::pair<std::string, DataTree>;
    using AttributesList = std::vector<KeyValuePair>;
    using ChildrenList = std::vector<KeyValuePair>;

    KeyValuePair parse_xml_element()
    {
        // Parse content
        DataTree result;
        std::vector<std::string_view> text_content;

        skip_whitespace();
        expect('<');

        strip_comment();

        // Parse tag name
        auto tag_name = std::string(parse_tag_name());
        if (root_name_.empty())
            root_name_ = tag_name;

        // Parse attributes
        AttributesList attributes;

        while (has_remaining_chars() && current_char() != '>' && current_char() != '/')
        {
            skip_whitespace();

            // Parse attribute name
            auto attr_name = parse_attribute_name();

            skip_whitespace();
            expect('=');
            skip_whitespace();

            // Parse attribute value (assuming it's quoted)
            auto attr_value = parse_attribute_value();

            if (attr_value.empty())
            {
                attributes.emplace_back(attr_name, DataTree(nullptr));
            }
            else
            {
                attributes.emplace_back(attr_name, convert_string_to_value(attr_value));
            }
        }

        // Parse children or text content
        ChildrenList children;

        if (current_char() == '/')
        {
            // Self-closing tag
            advance();
            expect('>');
        }
        else
        {
            expect('>');

            // Check for nested elements vs text content
            while (has_remaining_chars() && !(current_char() == '<' && next_char() == '/'))
            {
                strip_comment();
                if (current_char() == '<')
                {
                    // Nested element
                    children.push_back(parse_xml_element());
                }
                else
                {
                    // Text content
                    text_content.push_back(parse_text_content());
                }
                skip_whitespace();
            }

            // Parse closing tag
            expect('<');
            expect('/');
            auto closing_tag = parse_tag_name();
            expect('>');

            if (closing_tag != tag_name)
            {
                throw std::runtime_error(gul17::cat("Mismatched tags: ", tag_name, " vs ", closing_tag));
            }
        }

        // Determine how to represent this element - as object, array or simple value
        if (!attributes.empty() || !children.empty())
        {
            // Handle arrays for multiple same-tag children / attributes
            std::unordered_map<std::string, DataTree> obj;
            std::unordered_map<std::string, std::vector<DataTree>> child_groups;

            for (const auto& [child_tag, child_value] : children)
            {
                // For simplicity, assume each child is an object with its tag name
                child_groups[child_tag].push_back(child_value);
            }

            // Add grouped children to object
            for (auto& [child_tag, values] : child_groups)
            {
                if (values.size() == 1)
                {
                    obj[child_tag] = values[0];
                }
                else
                {
                    obj[child_tag] = DataTree(values);
                }
            }

            // Add attributes
            for (const auto& [attr_name, attr_value] : attributes)
            {
                auto key = "@" + attr_name;
                if (obj.find(key) != obj.end())
                {
                    throw std::runtime_error(gul17::cat("Duplicate attribute name: ", attr_name));
                }
                obj[key] = attr_value;
            }

            // Add text content if any
            if (!text_content.empty())
            {
                if (text_content.size() == 1)
                {
                    obj["#text"] = DataTree(std::string(text_content[0])); // Single text content
                }
                else
                {
                    /// FIXME: Better to pass string_views directly to DataTree?
                    DataTree::Array text_array;
                    std::transform(
                        text_content.begin(), text_content.end(),
                        std::back_inserter(text_array),
                        [](const std::string_view& txt) { return DataTree(std::string(txt)); });

                    obj["#text"] = DataTree(text_array); // Multiple text contents as array
                }
            }

            return std::make_pair(tag_name, DataTree(obj));
        }
        else if (!text_content.empty())
        {
            // Simple element with text content
            // Try to convert to appropriate type
            if (text_content.size() == 1)
            {
                return std::make_pair(tag_name, convert_string_to_value(text_content[0]));
            }
            else
            {
                throw std::runtime_error(gul17::cat("Multiple text contents in simple element: ", tag_name));
            }
        }
        else
        {
            // Empty element
            return std::make_pair(tag_name, DataTree(nullptr));
        }
    }

    void strip_comment()
    {
        skip_whitespace();

        if (current_char() == '<' && next_char() == '!' &&
            data_.compare(pos_, 4, "<!--") == 0)
        {
            pos_ += 4; // skip '<!--'
            while (has_remaining_chars() &&
                   data_.compare(pos_, 3, "-->") != 0)
            {
                ++pos_;
            }
            if (data_.compare(pos_, 3, "-->") == 0)
            {
                pos_ += 3; // skip '-->'
            }
            skip_whitespace();
        }
    }

    std::string_view parse_attribute_name()
    {
        auto start_pos = pos_;
        while (has_remaining_chars() && !std::isspace(current_char()) &&
               current_char() != '=' && current_char() != '>' && current_char() != '/')
        {
            ++pos_;
        }

        return data_.substr(start_pos, pos_ - start_pos);
    }

    std::string_view parse_attribute_value()
    {
        char quote_char = current_char();
        if (quote_char != '"' && quote_char != '\'')
            throw std::runtime_error("Expected quote for attribute value");

        advance(); // skip opening quote

        auto start_pos = pos_;
        while (has_remaining_chars() && current_char() != quote_char)
        {
            ++pos_;
        }
        auto value = data_.substr(start_pos, pos_ - start_pos);
        expect(quote_char); // skip closing quote

        return value;
    }

    std::string_view parse_tag_name()
    {
        auto start_pos = pos_;
        while (has_remaining_chars() && !std::isspace(current_char()) &&
               current_char() != '>' && current_char() != '/')
        {
            ++pos_;
        }

        return data_.substr(start_pos, pos_ - start_pos);
    }

    std::string_view parse_text_content()
    {
        size_t start_pos = pos_;
        while (has_remaining_chars() && current_char() != '<')
        {
            ++pos_;
        }
        auto text = data_.substr(start_pos, pos_ - start_pos);

        // Trim whitespace
        auto first = text.find_first_not_of(" \t\n\r");
        auto last = text.find_last_not_of(" \t\n\r");

        if (first == std::string::npos)
            return "";

        return text.substr(first, last - first + 1);
    }

    DataTree convert_string_to_value(const std::string_view& str)
    {
        // Try to convert to int
        try
        {
            size_t idx;
            int int_val = std::stoi(std::string(str), &idx);
            if (idx == str.size())
                return DataTree(int_val);
        }
        catch (...) {}

        // Try to convert to double
        try
        {
            size_t idx;
            double double_val = std::stod(std::string(str), &idx);
            if (idx == str.size())
                return DataTree(double_val);
        }
        catch (...) {}

        // Otherwise, return as string
        return DataTree(unescape_xml(str));
    }

    void skip_whitespace()
    {
        while (pos_ < data_.size() && std::isspace(data_[pos_]))
        {
            ++pos_;
        }
    }

    char current_char() const
    {
        return pos_ < data_.size() ? data_[pos_] : '\0';
    }

    char next_char() const
    {
        return pos_ + 1 < data_.size() ? data_[pos_ + 1] : '\0';
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

    static std::string unescape_xml(const std::string_view& str)
    {
        std::string result;

        size_t i = 0;
        while (i < str.length())
        {
            if (str[i] == '&')
            {
                if (str.compare(i, 5, "&amp;") == 0)
                {
                    result += '&';
                    i += 5;
                }
                else if (str.compare(i, 4, "&lt;") == 0)
                {
                    result += '<';
                    i += 4;
                }
                else if (str.compare(i, 4, "&gt;") == 0)
                {
                    result += '>';
                    i += 4;
                }
                else if (str.compare(i, 6, "&quot;") == 0)
                {
                    result += '"';
                    i += 6;
                }
                else if (str.compare(i, 6, "&apos;") == 0)
                {
                    result += '\'';
                    i += 6;
                }
                else
                {
                    result += '&';
                    ++i;
                }
            }
            else
            {
                result += str[i];
                ++i;
            }
        }

        return result;
    }

private:
    std::string_view data_;
    size_t pos_{0};
    std::string root_name_;
};

struct XmlDataSerializer
{
    XmlDataSerializer(const DataTree& tree_root)
        : tree_root_(tree_root)
    {}

    std::string serialize(size_t indent, const std::string& root_tag_name)
    {
        if (!tree_root_.is_object())
            throw std::runtime_error("Root value must be an object for XML serialization");
        serialize_value(tree_root_, root_tag_name, indent, 0);
        return output_.str();
    }

private:
    void serialize_value(const DataTree& value, const std::string& tag_name, size_t indent, size_t current_indent = 0)
    {
        std::string newline = indent > 0 ? "\n" : "";  // Add newlines if indenting
        std::string indent_str = std::string(current_indent, ' ');

        std::string opening_tag = "<" + tag_name;
        std::string closing_tag = "</" + tag_name + ">" + newline;

        if (value.is_null())
        {
            output_ << indent_str << opening_tag << "/>" << newline;
        }
        else if (value.is_boolean())
        {
            output_ << indent_str << opening_tag << ">"
                << (value.as<bool>() ? "true" : "false")
                << closing_tag;
        }
        else if (value.is_int())
        {
            output_ << indent_str << opening_tag << ">"
                << std::to_string(value.as<int>())
                << closing_tag;
        }
        else if (value.is_double())
        {
            output_ << indent_str << opening_tag << ">"
                << std::to_string(value.as<double>())
                << closing_tag;
        }
        else if (value.is_string())
        {
            output_ << indent_str << opening_tag << ">"
                << escape_xml(value.as<std::string>())
                << closing_tag;
        }
        else if (value.is_array())
        {
            const auto& array = value.as<DataTree::Array>();

            for (const auto& item : array)
            {
                serialize_value(item, tag_name, indent, current_indent);
            }
        }
        else if (value.is_object())
        {
            const auto& obj = value.as<DataTree::Object>();

            // Sort keys for consistent output
            std::vector<DataTree::Object::key_type> keys;
            std::transform(obj.begin(), obj.end(), std::back_inserter(keys),
                           [](const auto& pair) { return pair.first; });
            std::sort(keys.begin(), keys.end());

            // Opening tag with attributes
            output_ << indent_str << opening_tag;
            for (size_t i = 0; i < keys.size(); ++i)
            {
                const auto& key = keys[i];
                const auto& val = obj.at(key);

                if (key.rfind("@", 0) == 0)
                {
                    // Attribute
                    std::string attr_name = key.substr(1); // Strip '@'
                    output_ << " " << attr_name << "=\"";
                    if (val.is_null())
                    {
                        output_ << "\"";
                    }
                    else
                    {
                        output_ << escape_xml(val.as<std::string>()) << "\"";
                    }
                }
            }
            output_ << ">";
            output_ << newline;

            // Child elements and text content
            for (size_t i = 0; i < keys.size(); ++i)
            {
                const auto& key = keys[i];
                const auto& val = obj.at(key);

                // Skip already handled attributes and text content handled later
                if (key.rfind("@", 0) == 0 || key == "#text")
                    continue;

                serialize_value(val, key, indent, current_indent + indent);
            }

            auto it = obj.find("#text");
            if (it != obj.end() && it->second.is_string())
            {
                // Text content
                std::string next_indent_str = std::string(current_indent + indent, ' ');
                output_ << next_indent_str << escape_xml(it->second.as<std::string>()) << newline;
            }

            output_ << indent_str;
            output_ << closing_tag;
        }
    }

    static std::string escape_xml(const std::string_view& str)
    {
        std::string result;
        for (char c : str)
        {
            switch (c)
            {
                case '&': result += "&amp;"; break;
                case '<': result += "&lt;"; break;
                case '>': result += "&gt;"; break;
                case '"': result += "&quot;"; break;
                case '\'': result += "&apos;"; break;
                default: result += c; break;
            }
        }
        return result;
    }

private:
    const DataTree& tree_root_;
    std::ostringstream output_;
};

namespace gul17 {

DataTree from_xml_string(const std::string_view& data)
{
    XmlDataParser parser(data);
    return parser.parse();
}

std::string to_xml_string(const DataTree& value, size_t indent, const std::string& root_tag_name)
{
    XmlDataSerializer serializer(value);
    return serializer.serialize(indent, root_tag_name);
}

} // namespace gul17

// vi:ts=4:sw=4:sts=4:et
