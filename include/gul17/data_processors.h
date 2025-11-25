/**
 * \file   YamlDataProcessor.h
 * \author Jan Behrens
 * \date   Created on 20 November 2025
 * \brief  Declaration of the YamlDataProcessor class.
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
#include "gul17/traits.h"

#include <string>

namespace gul17 {

GUL_EXPORT
DataTree from_json_string(const std::string& data);

GUL_EXPORT
std::string to_json_string(const DataTree& value, size_t indent = 0);

GUL_EXPORT
DataTree from_xml_string(const std::string& data);

GUL_EXPORT
std::string to_xml_string(const DataTree& value, size_t indent = 0, const std::string& root_tag_name = "root");

GUL_EXPORT
DataTree from_yaml_string(const std::string& data);

GUL_EXPORT
std::string to_yaml_string(const DataTree& value, size_t indent = 0);

} // namespace gul17

#endif // GUL17_DATA_PROCESSORS_H_
