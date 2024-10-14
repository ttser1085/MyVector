cmake_minimum_required(VERSION 3.10)

project(MyVector)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_STANDARD 17)

include_directories(MyVector)
add_executable(MyVector main.cpp MyVector/MyVector.cpp MyVector/MyRAIterator.cpp)