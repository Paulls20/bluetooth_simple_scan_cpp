cmake_minimum_required(VERSION 3.12)
project(bluetooth_simple_scan)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG")

find_library(BLUETOOTH bluetooth)

add_executable(bluetooth_simple_scan main.cpp)

target_link_libraries(bluetooth_simple_scan "${BLUETOOTH}")

