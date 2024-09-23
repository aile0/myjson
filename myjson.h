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

#include <cstdint>
#include <iostream>
#include <map>    // store key-value pairs in json object
#include <regex>  // remove whitespace from json string
#include <string>
#include <variant>  // store different types of data in json
#include <vector>   // store elements in json array

namespace myjson {

class json;  // Forward declaration

using Null = std::monostate;
using Bool = bool;
using Int = int64_t;
using Float = double;
using String = std::string;
using Array = std::vector<json>;
using Object = std::map<std::string, json>;

class json {
   public:
    using Value = std::variant<Null, Bool, Int, Float, String, Array, Object>;
    enum class Type { Null, Bool, Int, Float, String, Array, Object };

   private:
    Type type;   // Type of json object
    Value data;  // Data stored in json object

    // Helper functions
    std::string get_type_as_string() const;

   public:
    // Constructors
    json() : type(Type::Null), data(Null{}) {};
    json(Null n) : type(Type::Null), data(n) {};
    json(Bool b) : type(Type::Bool), data(b) {};
    json(Int i) : type(Type::Int), data(i) {};
    json(Float f) : type(Type::Float), data(f) {};
    json(String s) : type(Type::String), data(s) {};
    json(const char* s) : type(Type::String), data(String(s)) {};
    json(const Array& a) : type(Type::Array), data(a) {};
    json(const Object& o) : type(Type::Object), data(o) {};

    json(const json& other) : type(other.type), data(other.data) {};
    json(json&& other) : type(other.type), data(std::move(other.data)) {};
    json& operator=(const json& other);
    json& operator=(json&& other);

    ~json() = default;

    // Accessors
    std::string get_type() const;
    Value get_data() const;

    // Operators
    json& operator[](const std::string& key);
    json& operator[](const char* key);
    json& operator[](size_t index);

    // Conversion functions
    std::string dump() const;
};
// Helper functions
std::string json::get_type_as_string() const {
    switch (type) {
        case Type::Null:
            return "Null";
        case Type::Bool:
            return "Bool";
        case Type::Int:
            return "Int";
        case Type::Float:
            return "Float";
        case Type::String:
            return "String";
        case Type::Array:
            return "Array";
        case Type::Object:
            return "Object";
        default:
            return "Unknown";
    }
    return std::string();
}

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
std::string json::get_type() const { return get_type_as_string(); }

json::Value json::get_data() const { return data; }

// Operators
json& json::operator[](const std::string& key) {
    if (type != Type::Object) {
        throw std::runtime_error("Error: json is not an object");
    }
    return std::get<Object>(data)[key];
}

json& json::operator[](const char* key) { return operator[](std::string(key)); }

json& json::operator[](size_t index) {
    if (type != Type::Array) {
        throw std::runtime_error("Error: json is not an array");
    }
    auto arr = std::get<Array>(data);
    if (index >= arr.size()) {
        throw std::runtime_error("Error: index out of range");
    } else {
        return arr[index];
    }
}

// Conversion functions
std::string json::dump() const {
    std::string result;
    switch (type) {
        case Type::Null:
            result = "null";
            break;
        case Type::Bool:
            result = std::get<Bool>(data) ? "true" : "false";
            break;
        case Type::Int:
            result = std::to_string(std::get<Int>(data));
            break;
        case Type::Float:
            result = std::to_string(std::get<Float>(data));
            break;
        case Type::String:
            result = "\"" + std::get<String>(data) + "\"";
            break;
        case Type::Array:
            result = "[";
            for (size_t i = 0; i < std::get<Array>(data).size(); ++i) {
                result += std::get<Array>(data)[i].dump();
                if (i != std::get<Array>(data).size() - 1) {
                    result += ", ";
                }
            }
            result += "]";
            break;
        case Type::Object:
            result = "{";
            for (auto it = std::get<Object>(data).begin();
                 it != std::get<Object>(data).end(); ++it) {
                result += "\"" + it->first + "\": " + it->second.dump();
                if (it != --std::get<Object>(data).end()) {
                    result += ", ";
                }
            }
            result += "}";
            break;
        default:
            result = "null";
    }
    return result;
}

// out of class functions
std::string remove_whitespace(const std::string& str) {
    std::regex re("\\s+");
    return std::regex_replace(str, re, "");
}

// Parse function
json parse(const std::string& str, size_t& index) {
    if (str[index] == 'n') {
        if (str.substr(index, 4) == "null") {
            index += 4;
            return json();
        }
    } else if (str[index] == 't') {
        if (str.substr(index, 4) == "true") {
            index += 4;
            return json(true);
        }
    } else if (str[index] == 'f') {
        if (str.substr(index, 5) == "false") {
            index += 5;
            return json(false);
        }
    } else if (str[index] == '\"') {
        size_t start = index + 1;
        size_t end = str.find('\"', start);
        index = end + 1;
        return json(str.substr(start, end - start));
    } else if (str[index] == '[') {
        Array arr;
        index++;
        while (str[index] != ']') {
            arr.push_back(parse(str, index));
            if (str[index] == ',') {
                index++;
            }
        }
        index++;
        return json(arr);
    } else if (str[index] == '{') {
        Object obj;
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
    } else if (std::isdigit(str[index]) || str[index] == '-') {
        size_t start = index;
        bool int_flag = true;
        while (std::isdigit(str[index]) || str[index] == '.' ||
               str[index] == 'e' || str[index] == 'E' || str[index] == '+' ||
               str[index] == '-') {
            index++;
        }
        return json(std::stod(str.substr(start, index - start)));
    }
    throw std::runtime_error("Error: invalid json string");
    return json();
}

json parse(const std::string& str) {
    std::string json_str = remove_whitespace(str);
    size_t index = 0;
    return parse(json_str, index);
}

json parse(const char* str) { return parse(std::string(str)); }

}  // namespace myjson