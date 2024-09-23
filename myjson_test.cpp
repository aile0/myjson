#include "myjson.h"

#include <iostream>

myjson::json foo() { return myjson::json(); }

int main() {
    // Test default constructor
    myjson::json j1;
    std::cout << "Default constructor: " << j1.get_type() << std::endl;

    // Test Bool constructor
    myjson::json j2(true);
    std::cout << "Bool constructor: " << j2.get_type() << " - "
              << std::get<myjson::Bool>(j2.get_data()) << std::endl;

    // Test Int constructor
    int64_t i = 42;
    myjson::json j5(i);
    std::cout << "Int constructor: " << j5.get_type() << " - "
              << std::get<myjson::Int>(j5.get_data()) << std::endl;

    // // Test Float constructor
    myjson::json j3(3.14);
    std::cout << "Float constructor: " << j3.get_type() << " - "
              << std::get<myjson::Float>(j3.get_data()) << std::endl;

    // // Test String constructor
    myjson::json j4("hello");
    std::cout << "String constructor: " << j4.get_type() << " - "
              << std::get<myjson::String>(j4.get_data()) << std::endl;

    std::cout << "hello, world!" << std::endl;
    return 0;
}