cmake_minimum_required(VERSION 3.14)
project(laba2 CXX)

# подключаем Catch2 как поддиректорию (из third_party/Catch2)
add_subdirectory(../third_party/Catch2 lab2_catch2_build)
add_library(iomanip INTERFACE)
target_include_directories(iomanip INTERFACE include)

file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/gprotect.cpp "#include <iomanip.hpp>\n") #создает файл gprotect.cpp с одной строкой "#include <iomanip.hpp>\n"

file(GLOB TESTS tests/*.cpp) # ищет все файлы .cpp в tests и добавляет в TESTS
add_executable(tests_iomanip ${TESTS} ${CMAKE_CURRENT_BINARY_DIR}/gprotect.cpp) #путь к текушей сборке
target_link_libraries(tests_iomanip iomanip Catch2::Catch2WithMain) #связывает тестовый файл с библиотеками
include(Catch) #подключение
catch_discover_tests(tests_iomanip) #все тесты
