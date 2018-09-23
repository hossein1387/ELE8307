cmake_minimum_required(VERSION 3.11)
project(cnn_binary)
 
include_directories(inc)
 
file(GLOB SOURCES "src/*.cpp")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(cnn_binary ${SOURCES})
# add_executable (hello hello.c)