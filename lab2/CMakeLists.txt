cmake_minimum_required(VERSION 3.14)
project(laba2 CXX)

# подключаем Catch2 как поддиректорию (из third_party/Catch2)
add_subdirectory(../third_party/Catch2 lab2_catch2_build)
add_library(iomanip INTERFACE)
target_include_directories(iomanip INTERFACE include)

file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/gprotect.cpp "#include <iomanip.hpp>\n")

file(GLOB TESTS tests/*.cpp)
add_executable(tests_iomanip ${TESTS} ${CMAKE_CURRENT_BINARY_DIR}/gprotect.cpp)
target_link_libraries(tests_iomanip iomanip Catch2::Catch2WithMain)
include(Catch)
catch_discover_tests(tests_iomanip)
