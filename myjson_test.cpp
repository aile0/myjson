#include "myjson.h"

#include <iostream>
#include <vector>

// Custom type conversion
// required to_json and from_json
class Person {
   public:
    std::string name;
    int age;
};

namespace myjson {
void to_json(myjson::json &j, const Person &p) {
    std::string tmp = p.name + "," + std::to_string(p.age);
    j = tmp;
}

void from_json(const myjson::json &j, Person &p) {
    p.name = j["name"].get<std::string>();
    p.age = j["age"].get<int64_t>();
}
}  // namespace myjson

int main() {
    // myjson::_null
    // null
    myjson::json j_null = myjson::make_json("null");
    // myjson::json j_bool
    // true or false
    myjson::json j_bool = myjson::make_json("true");
    // myjson::json j_int
    // int or int64_t
    myjson::json j_int = myjson::make_json("1");
    // myjson::json j_float
    // float or double
    myjson::json j_float = myjson::make_json("1.1");
    // myjson::json j_string
    // std::string
    myjson::json j_string = myjson::make_json("\"string\"");
    // myjson::json j_array
    // std::vector<json>
    myjson::json j_array = myjson::make_json("[1, 2, 3]");
    // myjson::json j_object
    // std::map<std::string, json>
    myjson::json j_object =
        myjson::make_json("{\"key1\": 1, \"key2\": \"value2\"}");

    // Standard Initialization Interface
    myjson::json j1 = myjson::parse("[1, 2, 3]");
    myjson::json j2 = myjson::make_json("{\"key1\": 1, \"key2\": \"value2\"}");

    // Output interface
    std::cout << "j1: " << j1 << std::endl;
    std::cout << "j2: " << j2 << std::endl;

    // Get value interface
    // The available types are myjson::_null, bool, int, int64_t, float, double,
    // std::string, myjson::_array, and myjson::_object.
    int key1 = j2["key1"].get<int64_t>();
    std::string key2 = j2["key2"].get<std::string>();
    std::cout << "key1: " << key1 << std::endl;
    std::cout << "key2: " << key2 << std::endl;

    // assign value interface (only for myjson::_object)
    // The available types are myjson::_null, bool, int, int64_t, float, double,
    // std::string, myjson::_array, and myjson::_object.
    j2["key1"] = 2;
    j2["key2"] = "value3";
    std::cout << "j2: " << j2 << std::endl;

    // assign value interface (only for myjson::_array)
    // The available types are myjson::_null, bool, int, int64_t, float, double,
    // std::string, myjson::_array, and myjson::_object.
    j1[0] = 2;
    j1[1] = 3;
    j1[2] = 4;
    std::cout << "j1: " << j1 << std::endl;

    // push value interface (only for myjson::_array)
    // The available types are myjson::_null, bool, int, int64_t, float, double,
    // std::string, myjson::_array, and myjson::_object.
    j1.push("agefa");
    std::cout << "j1: " << j1 << std::endl;

    // to_array interface
    // return std::vector<json>
    auto arr = j1.to_array();
    for (auto &i : arr) {
        std::cout << i << std::endl;
    }

    // to_map interface
    // return std::map<std::string, json>
    auto map = j2.to_map();
    for (auto &i : map) {
        std::cout << i.first << ": " << i.second << std::endl;
    }

    // Custom type conversion (only for j["key"] = custom_type or j[index] =
    // custom_type)
    Person p;
    p.name = "huizhihua";
    p.age = 25;

    myjson::json j_person =
        myjson::parse("{\"key1\": 1, \"key2\": \"value2\"}");
    j_person["person"] = p;
    std::cout << "j_person: " << j_person << std::endl;
    return 0;
}