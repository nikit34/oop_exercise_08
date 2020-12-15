cmake_minimum_required (VERSION 3.2)
set(CMAKE_CXX_STANDARD 17)
project(8_async)


add_executable(8_async main.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pthread")