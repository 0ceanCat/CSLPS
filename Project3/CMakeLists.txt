cmake_minimum_required(VERSION 3.17)
project(Project3)

set(CMAKE_CXX_STANDARD 14)

add_executable(Project3 source/main.cpp source/BitInputStream.cpp source/BitOutputStream.cpp source/Golomb.cpp)