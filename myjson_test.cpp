#include "myjson.h"

#include <iostream>

int main() {
    myjson::json j = myjson::parse("{\"key\": \"value\"}");
    j["name"] = "huizhihua";
    j["age"] = 18;
    j["height"] = 1.75;

    // std::cout << "name: " << n << std::endl;
    // std::cout << "age: " << a << std::endl;
    // std::cout << "height: " << h << std::endl;

    return 0;
}