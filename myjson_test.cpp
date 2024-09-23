#include "myjson.h"

#include <iostream>

myjson::json foo() { return myjson::json(); }

int main() {
    // Test default constructor
    myjson::json j1;
    std::cout << "j1: " << j1 << std::endl;

    // Test parse function
    myjson::json j2 = myjson::parse("{\"key\": \"value\"}");
    std::cout << "j2: " << j2 << std::endl;

    // Test make_json function
    myjson::json j3 = myjson::make_json("{\"key\": \"value\"}");
    std::cout << "j3: " << j3 << std::endl;

    myjson::json j4 = myjson::make_json("{\"key\": \"value\"}");
    std::cout << "type of j4: " << j4.get_type_as_string() << std::endl;

    myjson::json j5 = myjson::make_json("{\"key\": \"value\"}");
    j5["name"] = "myjson";
    j5["age"] = 20;

    j2["j1"] = j1;
    j3["j2"] = j2;
    j4["j3"] = j3;
    j5["j4"] = j4;
    std::cout << "j5: " << j5 << std::endl;

    return 0;
}