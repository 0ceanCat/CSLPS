cmake_minimum_required(VERSION 3.17)
project(Project2)

set(CMAKE_CXX_STANDARD 14)

add_executable(Project2 source/main.cpp source/BitInputStream.cpp source/BitOutputStream.cpp)