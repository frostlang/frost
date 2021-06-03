
cmake_minimum_required(VERSION 3.0)

project(Frost)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../build)

set(SOURCES
    ../src/main.cpp
)

add_executable(Frost ${SOURCES})