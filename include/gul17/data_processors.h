/**
 * \file   data_processors.h
 * \author Jan Behrens
 * \date   Created on 20 November 2025
 * \brief  Declaration of the data processor utility functions.
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
 * along with this program.  If not, see <https://www.gnu.or
 */

#ifndef GUL17_DATA_PROCESSORS_H_
#define GUL17_DATA_PROCESSORS_H_

#include "DataTree.h"

#include "gul17/internal.h"

#include <string>

namespace gul17 {

/**
 * \addtogroup data_processors_h gul17/data_processors.h
 * \brief Various data processor utility functions.
 * @{
 */

/**
 * Parse a JSON string and return the corresponding DataTree representation.
 *
 * The function parses the input JSON string and constructs a DataTree object
 * representing the hierarchical structure and data contained in the JSON.
 * Throws a std::runtime_error if the input string is not valid JSON.
 *
 * \code
 * auto a = from_json_string(R"({"foo": "bar"})");  // a == DataTree{"foo": "bar"}
 * \endcode
 *
 * \param data      The JSON string to be parsed.
 *
 * \see to_json_string()
 *
 * \since GUL version x.y.z
 */
GUL_EXPORT
DataTree from_json_string(const std::string_view& data);

/**
 * Serialize a DataTree object to a JSON string.
 *
 * The function serializes the given DataTree object into a JSON-formatted string.
 * The optional \c indent parameter specifies the number of spaces to use for
 * indentation in the output string (default is 0, meaning no pretty-printing).
 *
 * \code
 * auto a = to_json_string(DataTree{"foo": "bar"});  // a == "{\"foo\": \"bar\"}"
 * \endcode
 *
 * \param value     The DataTree object to be serialized.
 *
 * \see from_json_string()
 *
 * \since GUL version x.y.z
 */
GUL_EXPORT
std::string to_json_string(const DataTree& value, size_t indent = 0);

/**
 * Parse an XML string and return the corresponding DataTree representation.
 *
 * The function parses the input XML string and constructs a DataTree object
 * representing the hierarchical structure and data contained in the XML.
 * Throws a std::runtime_error if the input string is not valid XML.
 *
 * \code
 * auto a = from_xml_string(R"(<root><foo>bar</foo></root>)");  // a == DataTree{"foo": "bar"}
 * \endcode
 *
 * \param data      The XML string to be parsed.
 *
 * \see to_xml_string()
 *
 * \since GUL version x.y.z
 */
GUL_EXPORT
DataTree from_xml_string(const std::string_view& data);

/**
 * Serialize a DataTree object to an XML string.
 *
 * The function serializes the given DataTree object into a XML-formatted string.
 * The optional \c indent parameter specifies the number of spaces to use for
 * indentation in the output string (default is 0, meaning no pretty-printing).
 *
 * \code
 * auto a = to_xml_string(DataTree{"foo": "bar"});  // a == "<root><foo>bar</foo></root>"
 * \endcode
 *
 * \param value     The DataTree object to be serialized.
 *
 * \see from_xml_string()
 *
 * \since GUL version x.y.z
 */
GUL_EXPORT
std::string to_xml_string(const DataTree& value, size_t indent = 0,
                          const std::string& root_tag_name = "root");

/**
 * Parse an YAML string and return the corresponding DataTree representation.
 *
 * The function parses the input YAML string and constructs a DataTree object
 * representing the hierarchical structure and data contained in the YAML.
 * Throws a std::runtime_error if the input string is not valid YAML.
 *
 * \code
 * auto a = from_yaml_string(R"(foo: bar)");  // a == DataTree{"foo": "bar"}
 * \endcode
 *
 * \param data      The YAML string to be parsed.
 *
 * \see to_yaml_string()
 *
 * \since GUL version x.y.z
 */
GUL_EXPORT
DataTree from_yaml_string(const std::string_view& data);

/**
 * Serialize a DataTree object to a YAML string.
 *
 * The function serializes the given DataTree object into a YAML-formatted string.
 * The optional \c indent parameter specifies the number of spaces to use for
 * indentation in the output string (default is 2).
 *
 * \code
 * auto a = to_yaml_string(DataTree{"foo": "bar"});  // a == "foo: bar\n"
 * \endcode
 *
 * \param value     The DataTree object to be serialized.
 *
 * \see from_yaml_string()
 *
 * \since GUL version x.y.z
 */
GUL_EXPORT
std::string to_yaml_string(const DataTree& value, size_t indent = 2);

/// @}

} // namespace gul17

#endif // GUL17_DATA_PROCESSORS_H_

// vi:ts=4:sw=4:sts=4:et
