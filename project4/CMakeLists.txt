cmake_minimum_required(VERSION 3.17)
project(project4)

set(CMAKE_CXX_STANDARD 14)
add_executable(project4 source/main.cpp source/BitInputStream.cpp source/BitOutputStream.cpp source/Golomb.cpp source/Predictor.cpp header/Converter.h
        header/YUV420Converter.h header/YUV422Converter.h header/YUV444Converter.h)

set(OpenCV_DIR D:\\openCv\\opencv\\build2)
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(project4 ${OpenCV_LIBS})