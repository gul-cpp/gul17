/**
 * \file   test_XmlDataProcessor.cc
 * \author Jan Behrens
 * \date   Created on November 19, 2025
 * \brief  Test suite for the XmlDataProcessor class.
 *
 * \copyright Copyright 2019-2025 Deutsches Elektronen-Synchrotron (DESY), Hamburg
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

#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#if 0 // Temporarily disable all tests to focus on test_func()

using gul17::DataTree;
using gul17::from_xml_string;
using gul17::to_xml_string;

TEST_CASE("XmlDataProcessor: XML parsing", "[XmlDataProcessor]")
{
    auto tree = from_xml_string(
R"(<root>
    <key1>value1</key1>
    <key2>42</key2>
    <key3>1</key3>
    <key3>2</key3>
    <key3>3</key3>
    <key4>
        <nestedKey>3.1415</nestedKey>
    </key4>
    <key5/>
</root>)");

    REQUIRE(tree["key1"].is_string());
    REQUIRE(tree["key1"].as<std::string>() == "value1");

    REQUIRE(tree["key2"].is_number());
    REQUIRE(tree["key2"].as<int>() == 42);

    REQUIRE(tree["key3"].is_array());
    REQUIRE(tree["key3"].size() == 3);
    REQUIRE(tree["key3"][0].as<int>() == 1);
    REQUIRE(tree["key3"][1].as<int>() == 2);
    REQUIRE(tree["key3"][2].as<int>() == 3);

    REQUIRE(tree["key4"].is_object());
    REQUIRE(tree["key4"]["nestedKey"].is_double());
    REQUIRE(tree["key4"]["nestedKey"].as<double>() == Catch::Approx(3.1415));

    REQUIRE(tree["key5"].is_null());

    REQUIRE(tree.has_key("invalid") == false);
    REQUIRE(tree["invalid"].is_empty());
}

TEST_CASE("XmlDataProcessor: XML parsing with attributes and comments", "[XmlDataProcessor]")
{
    auto tree = from_xml_string(
R"(<root>
    <!-- ignored comment -->
    <key1 attr1="k1a1">value1</key1>
    <key2 attr1="k2a1" attr2="k2a2" attr3=""></key2>
    TEXT CONTENT
</root>)");

    REQUIRE(tree["key1"].is_object());
    REQUIRE(tree["key1"]["#text"].is_string());
    REQUIRE(tree["key1"]["#text"].as<std::string>() == "value1");
    REQUIRE(tree["key1"]["@attr1"].is_string());
    REQUIRE(tree["key1"]["@attr1"].as<std::string>() == "k1a1");

    REQUIRE(tree["key2"].is_object());
    REQUIRE(tree["key2"]["#text"].is_empty());
    REQUIRE(tree["key2"]["@attr1"].is_string());
    REQUIRE(tree["key2"]["@attr1"].as<std::string>() == "k2a1");
    REQUIRE(tree["key2"]["@attr2"].is_string());
    REQUIRE(tree["key2"]["@attr2"].as<std::string>() == "k2a2");
    REQUIRE(tree["key2"]["@attr3"].is_null());

    REQUIRE(tree["#text"].is_string());
    REQUIRE(tree["#text"].as<std::string>() == "TEXT CONTENT");
}

TEST_CASE("XmlDataProcessor: XML parsing with escape sequences", "[XmlDataProcessor]")
{
    auto tree = from_xml_string(
R"(
<root>
    <key1>&gt;&lt;&amp;&quot;&apos;</key1>
</root>
)");

    REQUIRE(tree["key1"].is_string());
    REQUIRE(tree["key1"].as<std::string>() == "><&\"'");
}

TEST_CASE("XmlDataProcessor: XML parsing with errors", "[XmlDataProcessor]")
{
    REQUIRE_THROWS(from_xml_string(R"(<root><key1></root>)")); // Missing closing tag
    REQUIRE_THROWS(from_xml_string(R"(<root><key1></key2></root>)")); // Mismatched closing tag
    REQUIRE_THROWS(from_xml_string(R"(<root><key1 attr1 attr2/></root>)")); // Malformed attribute
    REQUIRE_THROWS(from_xml_string(R"(<root><key1 attr1="1" attr1="2"/></root>)")); // Duplicate attribute
}

TEST_CASE("XmlDataProcessor: XML serialization", "[XmlDataProcessor]")
{
    auto tree = DataTree::make_object();

    tree["key1"] = "value1";
    tree["key2"] = 42;
    tree["key3"] = DataTree::Array{1, 2, 3};
    tree["key4"] = DataTree::Object{{"nestedKey", nullptr}};
    tree["key5"] = nullptr;

    std::string expected_xml =
R"(<root>
    <key1>value1</key1>
    <key2>42</key2>
    <key3>1</key3>
    <key3>2</key3>
    <key3>3</key3>
    <key4>
        <nestedKey/>
    </key4>
    <key5/>
</root>
)";

    auto xml_str = to_xml_string(tree, 4);
    REQUIRE(xml_str == expected_xml);
}

TEST_CASE("XmlDataProcessor: XML serialization with attributes", "[XmlDataProcessor]")
{
    auto tree = DataTree::make_object();

    tree["key1"]["#text"] = "value1";
    tree["key1"]["@attr1"] = "k1a1";
    tree["key2"]["#text"] = nullptr;
    tree["key2"]["@attr1"] = "k2a1";
    tree["key2"]["@attr2"] = "k2a2";
    tree["key2"]["@attr3"] = nullptr;
    tree["#text"] = "TEXT CONTENT";

    std::string expected_xml =
R"(<root>
    <key1 attr1="k1a1">
        value1
    </key1>
    <key2 attr1="k2a1" attr2="k2a2" attr3="">
    </key2>
    TEXT CONTENT
</root>
)";

    auto xml_str = to_xml_string(tree, 4);
    REQUIRE(xml_str == expected_xml);
}

TEST_CASE("XmlDataProcessor: XML parsing of SVR.AUTH string", "[XmlDataProcessor]")
{
    auto tree = from_xml_string(
R"(
<AUTH>
<OPER>         uid </OPER>         # set operator User  ID
<OPER_GROUP>   gid </OPER_GROUP>   # set operator Group ID
<XPERT>        uid </XPERT>        # set expert   User  ID
<XPERT_GROUP>  gid </XPERT_GROUP>  # set expert   Group ID
<CUSTOM>       uid </CUSTOM>       # set customer User  ID
<CUSTOM_GROUP> gid </CUSTOM_GROUP> # set customer Group ID

<USER0>        uid </USER0>        # set user 0   User  ID
<USER1>        uid </USER1>        # set user 1   User  ID
<USER2>        uid </USER2>        # set user 2   User  ID
<USER3>        uid </USER3>        # set user 3   User  ID
<USER4>        uid </USER4>        # set user 4   User  ID
<USER5>        uid </USER5>        # set user 5   User  ID
<USER6>        uid </USER6>        # set user 6   User  ID
<USER7>        uid </USER7>        # set user 7   User  ID
<USER8>        uid </USER8>        # set user 8   User  ID
<USER9>        uid </USER9>        # set user 9   User  ID
<USER10>       uid </USER10>       # set user 10  User  ID
<USER11>       uid </USER11>       # set user 11  User  ID
<USER12>       uid </USER12>       # set user 12  User  ID
<USER13>       uid </USER13>       # set user 13  User  ID
<USER14>       uid </USER14>       # set user 14  User  ID
<USER15>       uid </USER15>       # set user 15  User  ID

<LIST>
<ALL> mask </ALL>
<PERM><NAME> name </NAME><MASK> mask </MASK></PERM>
<PERM><NAME> name </NAME><MASK> mask </MASK></PERM>
</LIST>
</AUTH>
)");

    REQUIRE(tree["OPER"].as<std::string>() == "uid");
    REQUIRE(tree["OPER_GROUP"].as<std::string>() == "gid");
    // Other user/group IDs omitted for brevity

    REQUIRE(tree["LIST"]["PERM"].is_array());
    REQUIRE(tree["LIST"]["PERM"].size() == 2);
    REQUIRE(tree["LIST"]["PERM"][0]["NAME"].as<std::string>() == "name");
    REQUIRE(tree["LIST"]["PERM"][0]["MASK"].as<std::string>() == "mask");
    // Second PERM omitted for brevity
}

#endif
