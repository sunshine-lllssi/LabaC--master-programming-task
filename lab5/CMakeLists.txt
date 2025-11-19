cmake_minimum_required(VERSION 3.14)
project(variant)

find_package(Boost REQUIRED)
find_package(Catch2 REQUIRED)

add_library(variant INTERFACE)
target_include_directories(variant INTERFACE include)

file(GLOB TESTS tests/*.cpp)
add_executable(tests_variant ${TESTS})
target_link_libraries(tests_variant variant Catch2::Catch2WithMain Boost::boost)
include(Catch)
catch_discover_tests(tests_variant)