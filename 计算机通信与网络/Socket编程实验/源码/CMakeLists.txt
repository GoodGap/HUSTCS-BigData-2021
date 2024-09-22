cmake_minimum_required(VERSION 3.10)
project(main)

set(CMAKE_CXX_STANDARD 11)

add_executable(main main.cpp)
target_link_libraries(main wsock32 ws2_32)
