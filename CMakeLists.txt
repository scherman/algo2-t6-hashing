cmake_minimum_required(VERSION 3.5)
project(algo2_t6_hashing)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES test_dhc.cpp)
add_executable(algo2_t6_hashing ${SOURCE_FILES})