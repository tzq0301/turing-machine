cmake_minimum_required(VERSION 3.13)

project(turing)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

include_directories(turing-project)

add_executable(${CMAKE_PROJECT_NAME} turing-project/main.cpp)