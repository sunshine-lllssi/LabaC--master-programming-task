cmake_minimum_required(VERSION 3.17)
project(lab6)
set(CMAKE_CXX_STANDARD 20)

Include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.4.0 # or a later release
)

FetchContent_MakeAvailable(Catch2)
add_library(type_map INTERFACE)
target_include_directories(type_map INTERFACE include)
include(Catch)
file(GLOB TESTS tests/*.cpp)
add_executable(tests_type_map ${TESTS})
target_link_libraries(tests_type_map type_map Catch2::Catch2WithMain)
catch_discover_tests(tests_type_map)
