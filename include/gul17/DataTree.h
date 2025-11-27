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

/**
 * \addtogroup DataTree_h gul17/DataTree.h
 * \brief A hierarchical data structure for representing various data types.
 * @{
 */

/**
 * A hierarchical data structure that can represent various data types including
 * null, boolean, number, string, array, and object.
 *
 * The DataTree class can be used to create, manipulate, and access data in a
 * tree-like structure. It supports dynamic typing and can hold different types
 * of data at each node.
 * It can be used to represent data formats such as JSON, YAML, or XML.
 *
 * \code
 * // Create a data tree object
 * DataTree tree;
 *
 * tree["foo"] = "bar";          // String
 * tree["answer"] = 42;          // Number
 * tree["is_valid"] = true;     // Boolean
 * tree["items"] = DataTree::Array{1, 2, 3}; // Array
 * tree["config"] = DataTree::Object{ {"key1", "value1"}, {"key2", 42} }; // Object
 *
 * tree["items"].push_back(4);  // Add an element to the array
 * tree["config"]["key3"] = 3.14; // Add a key-value pair to the object
 * \endcode
 *
 * \since GUL version x.y.z
 */
class DataTree
{
public:
    // Type definitions

    /// Type of an object (key-value pairs)
    using Object = std::unordered_map<std::string, DataTree>;
    /// Type of an array (list of values)
    using Array = std::vector<DataTree>;
    /// Underlying variant type to hold different data types
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

    /**
     * Create an empty DataTree object (default to an empty object).
     */
    DataTree() : value_(Object()) {}  // default to an empty object

    /**
     * Create a DataTree object holding a null value.
     */
    DataTree(std::nullptr_t) : value_(nullptr) {}

    /**
     * Create a DataTree object holding a boolean value.
     */
    DataTree(bool b) : value_(b) {}

    /**
     * Create a DataTree object holding an integer value.
     */
    DataTree(int i) : value_(i) {}

    /**
     * Create a DataTree object holding a floating-point value.
     */
    DataTree(double d) : value_(d) {}

    /**
     * Create a DataTree object holding a string value.
     */
    DataTree(const std::string& s) : value_(s) {}

    /**
     * Create a DataTree object holding a C-style string value.
     */
    DataTree(const char* s) : value_(std::string(s)) {}

    /**
     * Create a DataTree object holding an array value.
     */
    DataTree(const Array& a) : value_(a) {}

    /**
     * Create a DataTree object holding an object value.
     */
    DataTree(const Object& o) : value_(o) {}

    // Factory methods

    /**
     * Create a DataTree object representing a null value.
     */
    static DataTree make_null() { return DataTree(nullptr); }

    /**
     * Create a DataTree object representing an empty array.
     */
    static DataTree make_array() { return DataTree(Array{}); }

    /**
     * Create a DataTree object representing an empty object.
     */
    static DataTree make_object() { return DataTree(Object{}); }

    // Move constructors

    /**
     * Create a DataTree object by moving an array into it.
     */
    DataTree(Array&& a) : value_(std::move(a)) {}

    /**
     * Create a DataTree object by moving an object into it.
     */
    DataTree(Object&& o) : value_(std::move(o)) {}

    /**
     * Create a DataTree object by moving a string into it.
     */
    DataTree(std::string&& s) : value_(std::move(s)) {}

    /**
     * Move another DataTree object into this one.
     */
    DataTree(DataTree&& other) = default;

    // Copy constructor

    /**
     * Create a copy of another DataTree object.
     */
    DataTree(const DataTree& other) = default;

    // Assignment operators

    /**
     * Assign another DataTree object to this one.
     */
    DataTree& operator=(const DataTree& other) = default;

    /**
     * Move-assign another DataTree object to this one.
     */
    DataTree& operator=(DataTree&& other) = default;

    // Type checking

    /**
     * Check if the DataTree holds a null value.
     * \return True if the DataTree is null, false otherwise.
     */
    bool is_null() const    { return std::holds_alternative<std::nullptr_t>(value_); }

    /**
     * Check if the DataTree holds a boolean value.
     * \return True if the DataTree is boolean, false otherwise.
     */
    bool is_boolean() const { return std::holds_alternative<bool>(value_); }

    /**
     * Check if the DataTree holds an integer value.
     * \return True if the DataTree is integer, false otherwise.
     */
    bool is_int() const     { return std::holds_alternative<int>(value_); }

    /**
     * Check if the DataTree holds a floating-point value.
     * \return True if the DataTree is floating-point, false otherwise.
     */
    bool is_double() const  { return std::holds_alternative<double>(value_); }

    /**
     * Check if the DataTree holds a numeric value.
     * \return True if the DataTree is numeric (integer or floating-point), false otherwise.
     */
    bool is_number() const  { return is_int() || is_double(); }

    /**
     * Check if the DataTree holds a string value.
     * \return True if the DataTree is string, false otherwise.
     */
    bool is_string() const  { return std::holds_alternative<std::string>(value_); }

    /**
     * Check if the DataTree holds an array value.
     * \return True if the DataTree is array, false otherwise.
     */
    bool is_array() const   { return std::holds_alternative<Array>(value_); }

    /**
     * Check if the DataTree holds an object value.
     * \return True if the DataTree is object, false otherwise.
     */
    bool is_object() const  { return std::holds_alternative<Object>(value_); }

    /**
     * Check if the DataTree is empty.
     * A DataTree is considered empty if it is null, or if it is a string, array,
     * or object that contains no elements.
     *
     * \return True if the DataTree is empty, false otherwise.
     */
    bool is_empty() const
    {
        if (is_null()) return true;
        if (is_string()) return std::get<std::string>(value_).empty();
        if (is_array()) return std::get<Array>(value_).empty();
        if (is_object()) return std::get<Object>(value_).empty();
        return false;
    }

    /**
     * Check if the object contains the specified key.
     * Throws a std::runtime_error if the DataTree is not an object.
     * \param key The key to check for.
     * \return True if the key exists, false otherwise.
     */
    bool has_key(const std::string& key) const
    {
        if (!is_object())
            throw std::runtime_error("DataTree is not an object");
        const auto& obj = std::get<Object>(value_);
        return obj.find(key) != obj.end();
    }

    /**
     * Get the size of the array or object.
     * Throws a std::runtime_error if the DataTree is neither an array nor an object.
     * \return The number of elements in the array or object.
     */
    size_t size() const
    {
        if (is_array())
            return std::get<Array>(value_).size();
        else if (is_object())
            return std::get<Object>(value_).size();
        else
            throw std::runtime_error("DataTree is neither array nor object");
    }

    /**
     * Add an element to the end of the array.
     * Throws a std::runtime_error if the DataTree is not an array.
     * \param val The value to add.
     */
    void push_back(const DataTree& val)
    {
        if (!is_array())
            throw std::runtime_error("DataTree is not an array");
        std::get<Array>(value_).push_back(val);
    }

    /**
     * Add an element to the end of the array by moving it.
     * Throws a std::runtime_error if the DataTree is not an array.
     * \param val The value to add.
     */
    void emplace_back(DataTree&& val)
    {
        if (!is_array())
            throw std::runtime_error("DataTree is not an array");
        std::get<Array>(value_).emplace_back(std::move(val));
    }

    /**
     * Insert a key-value pair into the object.
     * Throws a std::runtime_error if the DataTree is not an object.
     * \param key The key to insert.
     * \param val The value to insert.
     */
    void insert(const std::string& key, const DataTree& val)
    {
        if (!is_object())
            throw std::runtime_error("DataTree is not an object");
        std::get<Object>(value_)[key] = val;
    }

    /**
     * Insert a value into the array at the specified index.
     * Throws a std::runtime_error if the DataTree is not an array.
     * Throws a std::out_of_range if the index is out of bounds.
     * \param index The index at which to insert the value.
     * \param val   The value to insert.
     */
    void insert(size_t index, const DataTree& val)
    {
        if (!is_array())
            throw std::runtime_error("DataTree is not an array");
        auto& arr = std::get<Array>(value_);
        if (index > arr.size())
            throw std::out_of_range("Index out of range: " + std::to_string(index));
        arr.insert(arr.begin() + index, val);
    }

    /**
     * Clear all elements from the array or object.
     * Throws a std::runtime_error if the DataTree is neither an array nor an object.
     */
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

    /**
     * Return an iterator to the beginning of the array.
     * Throws a std::runtime_error if the DataTree is not an array.
     * \return An iterator to the first element.
     */
    iterator begin()
    {
        if (!is_array())
            throw std::runtime_error("DataTree is not an array");
        auto& arr = std::get<Array>(value_);
        return arr.data();
    }

    /**
     * Return an iterator to the end of the array.
     * Throws a std::runtime_error if the DataTree is not an array.
     * \return An iterator to the last element.
     */
    iterator end()
    {
        if (!is_array())
            throw std::runtime_error("DataTree is not an array");
        auto& arr = std::get<Array>(value_);
        return arr.data() + arr.size();
    }

    /**
     * Return a const iterator to the beginning of the array.
     * Throws a std::runtime_error if the DataTree is not an array.
     * \return A const iterator to the first element.
     */
    const_iterator cbegin() const { return const_cast<DataTree*>(this)->begin(); }

    /**
     * Return a const iterator to the end of the array.
     * Throws a std::runtime_error if the DataTree is not an array.
     * \return A const iterator to the last element.
     */
    const_iterator cend() const { return const_cast<DataTree*>(this)->end(); }

    /**
     * Return a const iterator to the beginning of the array.
     * Throws a std::runtime_error if the DataTree is not an array.
     * \return A const iterator to the first element.
     */
    const_iterator begin() const { return cbegin(); }

    /**
     * Return a const iterator to the end of the array.
     * Throws a std::runtime_error if the DataTree is not an array.
     * \return A const iterator to the last element.
     */
    const_iterator end() const { return cend(); }

    // Accessors with bounds checking

    /**
     * Get a reference to the value associated with the specified key in the object,
     * or the value at the specified index in the array.
     * Throws a std::runtime_error if the DataTree is not an object/array.
     * Throws a std::out_of_range if the key/index does not exist.
     * \param key   The key to look up in the object.
     * \param index The index to look up in the array.
     * \return A reference to the corresponding DataTree value.
     */
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

    /**
     * Get a const reference to the value associated with the specified key in the object.
     * Throws a std::runtime_error if the DataTree is not an object.
     * Throws a std::out_of_range if the key does not exist.
     * \param key The key to look up in the object.
     * \return A const reference to the corresponding DataTree value.
     */
    const DataTree& at(const std::string& key) const
    {
        return const_cast<DataTree*>(this)->at(key);
    }

    /**
     * Get a reference to the value at the specified index in the array.
     * Throws a std::runtime_error if the DataTree is not an array.
     * Throws a std::out_of_range if the index is out of bounds.
     * \param index The index to look up in the array.
     * \return A reference to the corresponding DataTree value.
     */
    DataTree& at(size_t index)
    {
        if (!is_array())
            throw std::runtime_error("DataTree is not an array");
        const auto& arr = std::get<Array>(value_);
        if (index >= arr.size())
            throw std::out_of_range("Index out of range: " + std::to_string(index));
        return const_cast<DataTree&>(arr[index]);
    }

    /**
     * Get a const reference to the value at the specified index in the array.
     * Throws a std::runtime_error if the DataTree is not an array.
     * Throws a std::out_of_range if the index is out of bounds.
     * \param index The index to look up in the array.
     * \return A const reference to the corresponding DataTree value.
     */
    const DataTree& at(size_t index) const
    {
        return const_cast<DataTree*>(this)->at(index);
    }

    // Operator[] without bounds checking

    /**
     * Get a reference to the value associated with the specified key in the object,
     * or the value at the specified index in the array.
     * Throws a std::runtime_error if the DataTree is not an object/array.
     * \param key   The key to look up in the object.
     * \param index The index to look up in the array.
     * \return A reference to the corresponding DataTree value.
     */
    DataTree& operator[](const std::string& key)
    {
        if (!is_object())
            throw std::runtime_error("DataTree is not an object");
        return std::get<Object>(value_)[key];
    }

    /**
     * Get a const reference to the value associated with the specified key in the object,
     * or the value at the specified index in the array.
     * Throws a std::runtime_error if the DataTree is not an object/array.
     * \param key   The key to look up in the object.
     * \param index The index to look up in the array.
     * \return A const reference to the corresponding DataTree value.
     */
    const DataTree& operator[](const std::string& key) const
    {
        return (*const_cast<DataTree*>(this))[key];
    }

    /**
     * Get a reference to the value at the specified index in the array.
     * Throws a std::runtime_error if the DataTree is not an array.
     * \param index The index to look up in the array.
     * \return A reference to the corresponding DataTree value.
     */
    DataTree& operator[](size_t index)
    {
        if (!is_array())
            throw std::runtime_error("DataTree is not an array");
        return std::get<Array>(value_)[index];
    }

    /**
     * Get a const reference to the value at the specified index in the array.
     * Throws a std::runtime_error if the DataTree is not an array.
     * \param index The index to look up in the array.
     * \return A reference to the corresponding DataTree value.
     */
    const DataTree& operator[](size_t index) const
    {
        return (*const_cast<DataTree*>(this))[index];
    }

    // Type checking for template types

    /**
     * Check if the DataTree holds a value of type T.
     * \tparam T The type to check against.
     * \return True if the DataTree holds a value of type T, false otherwise.
     */
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

    /**
     * Convert the DataTree to the specified type T.
     * Throws a std::bad_variant_access if the conversion is not possible.
     * \tparam T The type to convert to.
     * \return The converted value.
     *
     * \code
     * DataTree tree = 42;
     * int value = tree.as<int>(); // value == 42
     * \endcode
     *
     * Note that implicit conversions are supported for some types:
     * - int to double
     * - boolean to int
     * - int/double/boolean/null to string
     */
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
            if (is_int()) return std::get<int>(value_) != 0;
        }
        else if constexpr (std::is_same_v<T, int>)
        {
            if (is_int()) return std::get<int>(value_);
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
            // TODO: Add conversion logic for other types to string if needed
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

    /**
     * Get a reference to the underlying value variant.
     * \return A reference to the underlying Value variant.
     */
    Value& get_value() { return value_; }

    /**
     * Get a const reference to the underlying value variant.
     * \return A const reference to the underlying Value variant.
     */
    const Value& get_value() const { return value_; }

private:
    Value value_;
};

/// @}

} // namespace gul17

#endif // GUL17_DATA_TREE_H_

// vi:ts=4:sw=4:sts=4:et
