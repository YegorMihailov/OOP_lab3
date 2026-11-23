cmake_minimum_required(VERSION 3.16)
project(lab3_variant3)

set(CMAKE_CXX_STANDARD 20)

add_library(fraction_lib src/fraction_set.cpp)
target_include_directories(fraction_lib PUBLIC include)

add_executable(lab3_main src/main.cpp)
target_link_libraries(lab3_main PRIVATE fraction_lib)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(googletest)

add_executable(lab3_tests tests/test.cpp)
target_link_libraries(lab3_tests PRIVATE fraction_lib GTest::gtest_main)