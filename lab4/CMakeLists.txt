cmake_minimum_required(VERSION 3.14)
project(lab4)

find_package(Boost REQUIRED)
find_package(Catch2 REQUIRED)
add_library(iterator INTERFACE)
target_include_directories(iterator INTERFACE include)
target_link_libraries(iterator INTERFACE Boost::boost)

file(GLOB TESTS tests/*.cpp)
add_executable(tests_iterator ${TESTS})
target_link_libraries(tests_iterator iterator Catch2::Catch2WithMain)
include(Catch)
catch_discover_tests(tests_iterator)
