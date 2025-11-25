/**
 * \file   DataTree.h
 * \author Jan Behrens
 * \date   Created on 19 November 2025
 * \brief  Declaration of the DataTree class.
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

#ifndef GUL17_DATA_TREE_H_
#define GUL17_DATA_TREE_H_

#include <stdexcept>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

namespace gul17 {

class DataTree
{
public:
    using Object = std::unordered_map<std::string, DataTree>;
    using Array = std::vector<DataTree>;
    using Value = std::variant<
        std::nullptr_t,    // null
        bool,              // boolean
        int,               // integer
        double,            // float
        std::string,       // string
        Array,             // array
        Object             // object
    >;

    // Constructors
    DataTree() : value_(Object()) {}  // default to an empty object
    DataTree(std::nullptr_t) : value_(nullptr) {}
    DataTree(bool b) : value_(b) {}
    DataTree(int i) : value_(i) {}
    DataTree(double d) : value_(d) {}
    DataTree(const std::string& s) : value_(s) {}
    DataTree(const char* s) : value_(std::string(s)) {}
    DataTree(const Array& a) : value_(a) {}
    DataTree(const Object& o) : value_(o) {}

    // Factory methods
    static DataTree make_array() { return DataTree(Array{}); }
    static DataTree make_object() { return DataTree(Object{}); }

    // Move constructors
    DataTree(Array&& a) : value_(std::move(a)) {}
    DataTree(Object&& o) : value_(std::move(o)) {}
    DataTree(std::string&& s) : value_(std::move(s)) {}

    // Copy constructor
    DataTree(const DataTree& other) = default;

    // Move constructor
    DataTree(DataTree&& other) = default;

    // Assignment operators
    DataTree& operator=(const DataTree& other) = default;
    DataTree& operator=(DataTree&& other) = default;

    // Type checking
    bool is_null() const    { return std::holds_alternative<std::nullptr_t>(value_); }
    bool is_boolean() const { return std::holds_alternative<bool>(value_); }
    bool is_int() const     { return std::holds_alternative<int>(value_); }
    bool is_double() const  { return std::holds_alternative<double>(value_); }
    bool is_number() const  { return is_int() || is_double(); }
    bool is_string() const  { return std::holds_alternative<std::string>(value_); }
    bool is_array() const   { return std::holds_alternative<Array>(value_); }
    bool is_object() const  { return std::holds_alternative<Object>(value_); }

    bool is_empty() const
    {
        if (is_null()) return true;
        if (is_string()) return std::get<std::string>(value_).empty();
        if (is_array()) return std::get<Array>(value_).empty();
        if (is_object()) return std::get<Object>(value_).empty();
        return false;
    }

    bool has_key(const std::string& key) const
    {
        if (!is_object())
            throw std::runtime_error("DataTree is not an object");
        const auto& obj = std::get<Object>(value_);
        return obj.find(key) != obj.end();
    }

    size_t size() const
    {
        if (is_array())
            return std::get<Array>(value_).size();
        else if (is_object())
            return std::get<Object>(value_).size();
        else
            throw std::runtime_error("DataTree is neither array nor object");
    }

    void push_back(const DataTree& val)
    {
        if (!is_array())
            throw std::runtime_error("DataTree is not an array");
        std::get<Array>(value_).push_back(val);
    }

    void emplace_back(DataTree&& val)
    {
        if (!is_array())
            throw std::runtime_error("DataTree is not an array");
        std::get<Array>(value_).emplace_back(std::move(val));
    }

    void insert(const std::string& key, const DataTree& val)
    {
        if (!is_object())
            throw std::runtime_error("DataTree is not an object");
        std::get<Object>(value_)[key] = val;
    }

    void insert(size_t index, const DataTree& val)
    {
        if (!is_array())
            throw std::runtime_error("DataTree is not an array");
        auto& arr = std::get<Array>(value_);
        if (index > arr.size())
            throw std::out_of_range("Index out of range: " + std::to_string(index));
        arr.insert(arr.begin() + index, val);
    }

    void clear()
    {
        if (is_array())
            std::get<Array>(value_).clear();
        else if (is_object())
            std::get<Object>(value_).clear();
        else
            throw std::runtime_error("DataTree is neither array nor object");
    }

    // Iterator return types, only works for arrays
    using iterator = DataTree*;
    using const_iterator = const DataTree*;

    // Iterators
    iterator begin()
    {
        if (!is_array())
            throw std::runtime_error("DataTree is not an array");
        auto& arr = std::get<Array>(value_);
        return arr.data();
    }

    iterator end()
    {
        if (!is_array())
            throw std::runtime_error("DataTree is not an array");
        auto& arr = std::get<Array>(value_);
        return arr.data() + arr.size();
    }

    const_iterator cbegin() const { return const_cast<DataTree*>(this)->begin(); }
    const_iterator cend() const { return const_cast<DataTree*>(this)->end(); }

    const_iterator begin() const { return cbegin(); }
    const_iterator end() const { return cend(); }

    // Accessors with bounds checking
    DataTree& at(const std::string& key)
    {
        if (!is_object())
            throw std::runtime_error("DataTree is not an object");
        const auto& obj = std::get<Object>(value_);
        auto it = obj.find(key);
        if (it == obj.end())
            throw std::out_of_range("Key not found in object: " + key);
        return const_cast<DataTree&>(it->second);
    }
    const DataTree& at(const std::string& key) const
    {
        return const_cast<DataTree*>(this)->at(key);
    }
    DataTree& at(size_t index)
    {
        if (!is_array())
            throw std::runtime_error("DataTree is not an array");
        const auto& arr = std::get<Array>(value_);
        if (index >= arr.size())
            throw std::out_of_range("Index out of range: " + std::to_string(index));
        return const_cast<DataTree&>(arr[index]);
    }
    const DataTree& at(size_t index) const
    {
        return const_cast<DataTree*>(this)->at(index);
    }

    // Operator[] without bounds checking
    DataTree& operator[](const std::string& key)
    {
        if (!is_object())
            throw std::runtime_error("DataTree is not an object");
        return std::get<Object>(value_)[key];
    }
    const DataTree& operator[](const std::string& key) const
    {
        return (*const_cast<DataTree*>(this))[key];
    }
    DataTree& operator[](size_t index)
    {
        if (!is_array())
            throw std::runtime_error("DataTree is not an array");
        return std::get<Array>(value_)[index];
    }
    const DataTree& operator[](size_t index) const
    {
        return (*const_cast<DataTree*>(this))[index];
    }

    template<typename T>
    bool is() const
    {
        if constexpr (std::is_same_v<T, std::nullptr_t>)
        {
            return is_null();
        }
        else if constexpr (std::is_same_v<T, bool>)
        {
            return is_boolean();
        }
        else if constexpr (std::is_same_v<T, int>)
        {
            return is_int();
        }
        else if constexpr (std::is_same_v<T, double>)
        {
            return is_double();
        }
        else if constexpr (std::is_same_v<T, std::string>)
        {
            return is_string();
        }
        else if constexpr (std::is_same_v<T, DataTree::Array>)
        {
            return is_array();
        }
        else if constexpr (std::is_same_v<T, DataTree::Object>)
        {
            return is_object();
        }
        return false;
    }

    // Conversion
    template<typename T>
    T as() const
    {
        if constexpr (std::is_same_v<T, std::nullptr_t>)
        {
            if (is_null()) return std::get<std::nullptr_t>(value_);
        }
        else if constexpr (std::is_same_v<T, bool>)
        {
            if (is_boolean()) return std::get<bool>(value_);
        }
        else if constexpr (std::is_same_v<T, int>)
        {
            if (is_int()) return std::get<int>(value_);
            if (is_double()) return static_cast<int>(std::get<double>(value_));
            if (is_boolean()) return static_cast<int>(std::get<bool>(value_));
        }
        else if constexpr (std::is_same_v<T, double>)
        {
            if (is_double()) return std::get<double>(value_);
            if (is_int()) return static_cast<double>(std::get<int>(value_));
        }
        else if constexpr (std::is_same_v<T, std::string>)
        {
            if (is_string()) return std::get<std::string>(value_);
            if (is_int()) return std::to_string(std::get<int>(value_));
            if (is_double()) return std::to_string(std::get<double>(value_));
            if (is_boolean()) return std::get<bool>(value_) ? "true" : "false";
            if (is_null()) return "null";
            // Add conversion logic for other types to string if needed
        }
        else if constexpr (std::is_same_v<T, DataTree::Array>)
        {
            if (is_array()) return std::get<Array>(value_);
        }
        else if constexpr (std::is_same_v<T, DataTree::Object>)
        {
            if (is_object()) return std::get<Object>(value_);
        }

        throw std::bad_variant_access();
    }

    // Get underlying value
    Value& get_value() { return value_; }
    const Value& get_value() const { return value_; }

private:
    Value value_;
};

} // namespace gul17

#endif // GUL17_DATA_TREE_H_
