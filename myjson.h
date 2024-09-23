/********************************************************************
 * Filename: myjson.h
 * Description: This is a header file for a simple JSON library.
 *              It defines a json class that can store and manipulate
 *              JSON data. Supported data types include Null, Bool,
 *              Float, String, Array, and Object. The library also
 *              provides some helper functions to handle JSON data.
 * Author: ile
 * Date: 2024-09-22
 ********************************************************************/
#pragma once

#include <cstdint>    // int64_t
#include <exception>  // runtime_error
#include <iostream>
#include <map>    // store key-value pairs in json object
#include <regex>  // remove whitespace from json string
#include <string>
#include <variant>  // store different types of data in json
#include <vector>   // store elements in json array

namespace myjson {

class json;  // Forward declaration

using _null = std::monostate;
using _bool = bool;
using _int = int64_t;
using _float = double;
using _string = std::string;
using _array = std::vector<json>;
using _object = std::map<std::string, json>;

class json {
   public:
    using Value =
        std::variant<_null, _bool, _int, _float, _string, _array, _object>;
    enum class Type { _null, _bool, _int, _float, _string, _array, _object };

   private:
    Type type;   // Type of json object
    Value data;  // Data stored in json object

    // Helper functions

   public:
    // Constructors
    json() : type(Type::_null), data(_null()) {};
    json(bool value) : type(Type::_bool), data(value) {};
    json(int value) : type(Type::_int), data(static_cast<int64_t>(value)) {};
    json(int64_t value) : type(Type::_int), data(value) {};
    json(float value) : type(Type::_float), data(static_cast<double>(value)) {};
    json(double value) : type(Type::_float), data(value) {};
    json(const std::string& value) : type(Type::_string), data(value) {};
    json(const char* value)
        : type(Type::_string), data(static_cast<std::string>(value)) {};
    json(const _array& value) : type(Type::_array), data(value) {};
    json(const _object& value) : type(Type::_object), data(value) {};

    json(const json& other) : type(other.type), data(other.data) {};
    json(json&& other) : type(other.type), data(std::move(other.data)) {};
    json& operator=(const json& other);
    json& operator=(json&& other);

    ~json() = default;

    // Accessors
    Type get_type() const;
    std::string get_type_as_string() const;
    Value get_data() const;

    // template <class T>
    // T get() {
    //     return std::visit(
    //         [](auto&& arg) -> T {
    //             using ArgType = decltype(arg);
    //             if constexpr (std::is_same_v<T, ArgType>) {
    //                 return arg;
    //             } else {
    //                 T result;
    //                 from_json(arg, result);
    //                 return result;
    //             }
    //         },
    //         data);
    // }

    // template <class T>
    // void get_to(T& other) const {
    //     if (std::holds_alternative<T>(data)) {
    //         other = std::get<T>(data);
    //     } else {
    //         throw std::runtime_error("Error: invalid type");
    //     }
    // }

    auto to_array() const -> _array {
        if (type == Type::_array) {
            return std::get<_array>(data);
        } else {
            throw std::runtime_error("Error: json is not an array");
        }
    }

    auto to_map() const -> _object {
        if (type == Type::_object) {
            return std::get<_object>(data);
        } else {
            throw std::runtime_error("Error: json is not an object");
        }
    }

    // Operators
    json& operator[](const std::string& key);
    json& operator[](const char* key);
    json& operator[](size_t index);

    // operator overloading
    friend std::ostream& operator<<(std::ostream& os, const json& j);

    // Conversion functions
    std::string dump() const;
};
// Helper functions

// Constructors
json& json::operator=(const json& other) {
    type = other.type;
    data = other.data;
    return *this;
}

json& json::operator=(json&& other) {
    type = other.type;
    data = std::move(other.data);
    return *this;
}

// Accessors
json::Type json::get_type() const { return type; }

std::string json::get_type_as_string() const {
    if (type == Type::_null)
        return "null";
    else if (type == Type::_bool)
        return "bool";
    else if (type == Type::_int)
        return "int";
    else if (type == Type::_float)
        return "float";
    else if (type == Type::_string)
        return "string";
    else if (type == Type::_array)
        return "array";
    else if (type == Type::_object)
        return "object";
    else
        return std::string("Unknown type");
}

json::Value json::get_data() const { return data; }

// Operators
json& json::operator[](const std::string& key) {
    if (type != Type::_object) {
        throw std::runtime_error("Error: json is not an object");
    }
    return std::get<_object>(data)[key];
}

json& json::operator[](const char* key) { return operator[](std::string(key)); }

json& json::operator[](size_t index) {
    if (type != Type::_array) {
        throw std::runtime_error("Error: json is not an array");
    }
    auto arr = std::get<_array>(data);
    if (index >= arr.size()) {
        throw std::runtime_error("Error: index out of range");
    } else {
        return arr[index];
    }
}

// operator overloading
std::ostream& operator<<(std::ostream& os, const json& j) {
    os << j.dump();
    return os;
}

// Conversion functions
std::string json::dump() const {
    std::string result;
    if (type == Type::_null) {
        result = "null";
    } else if (type == Type::_bool) {
        result = std::get<_bool>(data) ? "true" : "false";
    } else if (type == Type::_int) {
        result = std::to_string(std::get<_int>(data));
    } else if (type == Type::_float) {
        result = std::to_string(std::get<_float>(data));
    } else if (type == Type::_string) {
        result = "\"" + std::get<_string>(data) + "\"";
    } else if (type == Type::_array) {
        result = "[";
        for (size_t i = 0; i < std::get<_array>(data).size(); ++i) {
            result += std::get<_array>(data)[i].dump();
            if (i != std::get<_array>(data).size() - 1) {
                result += ", ";
            }
        }
        result += "]";
    } else if (type == Type::_object) {
        result = "{";
        for (auto it = std::get<_object>(data).begin();
             it != std::get<_object>(data).end(); ++it) {
            result += "\"" + it->first + "\": " + it->second.dump();
            if (it != --std::get<_object>(data).end()) {
                result += ", ";
            }
        }
        result += "}";
    } else {
        result = "Unknown type";
    }
    return result;
}

// out of class functions
std::string remove_whitespace(const std::string& str) {
    std::regex re("\\s+");
    return std::regex_replace(str, re, "");
}

// Parse function
json parse(const std::string& str, size_t& index);

json parse_null(const std::string& str, size_t& index) {
    if (str.substr(index, 4) == "null") {
        index += 4;
        return json();
    } else {
        throw std::runtime_error("Error: invalid null value");
    }
}

json parse_true(const std::string& str, size_t& index) {
    if (str.substr(index, 4) == "true") {
        index += 4;
        return json(true);
    } else {
        throw std::runtime_error("Error: invalid true value");
    }
}

json parse_false(const std::string& str, size_t& index) {
    if (str.substr(index, 5) == "false") {
        index += 5;
        return json(false);
    } else {
        throw std::runtime_error("Error: invalid false value");
    }
}

json parse_string(const std::string& str, size_t& index) {
    try {
        size_t start = index + 1;
        size_t end = str.find('\"', start);
        index = end + 1;
        return json(str.substr(start, end - start));
    } catch (std::exception& e) {
        throw std::runtime_error("Error: invalid string value");
    }
}

json parse_array(const std::string& str, size_t& index) {
    try {
        _array arr;
        index++;
        while (str[index] != ']') {
            arr.push_back(parse(str, index));
            if (str[index] == ',') {
                index++;
            }
        }
        index++;
        return json(arr);
    } catch (std::exception& e) {
        throw std::runtime_error("Error: invalid array value");
    }
}

json parse_object(const std::string& str, size_t& index) {
    try {
        _object obj;
        index++;
        while (str[index] != '}') {
            std::string key =
                std::get<std::string>(parse(str, index).get_data());
            index++;
            obj[key] = parse(str, index);
            if (str[index] == ',') {
                index++;
            }
        }
        index++;
        return json(obj);
    } catch (std::exception& e) {
        throw std::runtime_error("Error: invalid object value");
    }
}

json parse_number(const std::string& str, size_t& index) {
    try {
        size_t start = index;
        bool int_flag = true;
        while (std::isdigit(str[index]) || str[index] == '.' ||
               str[index] == 'e' || str[index] == 'E' || str[index] == '+' ||
               str[index] == '-') {
            index++;
        }
        return json(std::stod(str.substr(start, index - start)));
    } catch (const std::exception& e) {
        throw std::runtime_error("Error: invalid number value");
    }
}

json parse(const std::string& str, size_t& index) {
    try {
        if (str[index] == 'n') {
            return parse_null(str, index);
        } else if (str[index] == 't') {
            return parse_true(str, index);
        } else if (str[index] == 'f') {
            return parse_false(str, index);
        } else if (str[index] == '\"') {
            return parse_string(str, index);
        } else if (str[index] == '[') {
            return parse_array(str, index);
        } else if (str[index] == '{') {
            return parse_object(str, index);
        } else if (std::isdigit(str[index]) || str[index] == '-') {
            return parse_number(str, index);
        } else {
            throw std::runtime_error("Error: Unknown character");
        }
    } catch (std::exception& e) {
        throw std::runtime_error(
            "Error: invalid JSON string in parse() at position " +
            std::to_string(index) + "\n" + e.what());
    }
    return json();
}

json parse(const std::string& str) {
    std::string json_str = remove_whitespace(str);
    size_t index = 0;
    return parse(json_str, index);
}

json parse(const char* str) { return parse(std::string(str)); }

json make_json(const std::string& str) { return parse(str); }

json make_json(const char* str) { return parse(str); }

}  // namespace myjson