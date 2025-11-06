cmake_minimum_required(VERSION 3.14)  

project(proxy_lab LANGUAGES CXX)
find_package(Threads REQUIRED)
find_package(Catch2 3 REQUIRED)
add_library(proxy INTERFACE)
target_include_directories(proxy INTERFACE include)
target_link_libraries(proxy INTERFACE Threads::Threads)

file(GLOB TESTS tests/*.cpp)
add_executable(tests_proxy ${TESTS})
target_link_libraries(tests_proxy PRIVATE proxy Catch2::Catch2WithMain)
include(Catch)
catch_discover_tests(tests_proxy)
