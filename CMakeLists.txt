cmake_minimum_required(VERSION 3.12)
project(ros_bluetooth)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG")

find_library(BLUETOOTH bluetooth)

add_executable(ros_bluetooth main.cpp)

target_link_libraries(ros_bluetooth "${BLUETOOTH}")