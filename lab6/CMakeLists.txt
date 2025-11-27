cmake_minimum_required(VERSION 3.14)
project(json_parser_lab LANGUAGES CXX)

find_package(Catch2 REQUIRED)

add_library(json_parser INTERFACE)
target_include_directories(json_parser INTERFACE include)  

file(GLOB TESTS tests/*.cpp)
add_executable(tests_json_parser ${TESTS})
target_link_libraries(tests_json_parser json_parser Catch2::Catch2WithMain)

include(Catch)
catch_discover_tests(tests_json_parser) 