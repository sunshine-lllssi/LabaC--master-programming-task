/* Json object.
 * @file
 * @date 2018-08-09
 * @author Anonymous
 */

#ifndef __JSON_HPP__
#define __JSON_HPP__

#include <map>
#include <string>
#include <vector>

#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <boost/fusion/adapted/std_pair.hpp>

/* Json EBNF specification (https://www.json.org)
 *      string := \" (\\ . | [^\"])* \"
 *      number := [0-9]+(\.[0-9]*)?
 *      boolean := "true" | "false"
 *      nullable := "null"
 *      value := number | string | boolean | nullable | array | object
 *      array := '[' value (',' value)* ']'
 *      key_value := string ':' value
 *      object := '{' key_value (',' key_value)* '}'
 *      json := array | object
 */

namespace x3 = boost::spirit::x3;

namespace json::types
{
    //{ describe json data types
    struct value;

    using array = std::vector<value>;
    using object = std::map<std::string, value>;

    using json = x3::variant<array, object>;

    struct value: x3::variant<
        nullptr_t,
        bool,
        int,
        float,
        std::string,
        x3::forward_ast<array>,
        x3::forward_ast<object>
    >
    {
        using base_type::base_type;
        using base_type::operator=;
    };
    //}
}

namespace json::parser
{
    const auto sfloat_ = x3::real_parser<float, x3::strict_real_policies<float>>();

    //{ describe json grammar
    const auto number = sfloat_ | x3::int_;
    const auto nullable = x3::lit("null") >> x3::attr(nullptr);
    
    x3::rule<struct quoted_string_class, std::string> const quoted_string = "quoted_string";
    const auto quoted_string_def = x3::lexeme['"' >> *(('\\' > x3::char_) | ~x3::char_('"')) >> '"'];

    x3::rule<struct array_class, types::array> const array = "array";
    x3::rule<struct object_class, types::object> const object = "object";
    x3::rule<struct json_class, types::json> const json = "json";
    x3::rule<struct value_class, types::value> const value = "value";

    const auto key_value = quoted_string >> ':' >> value;

    const auto array_def = '[' >> -(value % ',') >> ']';
    const auto object_def = '{' >> -(key_value % ',') >> '}';
    const auto json_def = array | object;

    const auto value_def = nullable | x3::bool_ | number | quoted_string | array | object;
    //}

    BOOST_SPIRIT_DEFINE(quoted_string, array, object, json, value)
}

#endif // __JSON_HPP__