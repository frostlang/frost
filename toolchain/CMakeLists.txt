
cmake_minimum_required(VERSION 3.0)

project(Frost)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../build)

include_directories(../src)

set(SOURCES
    ../src/main.cpp
    ../src/debug.cpp
    ../src/type.cpp
    ../src/types.cpp
    ../src/parse/token.cpp
    ../src/parse/lexer.cpp
    ../src/parse/parser.cpp
    ../src/parse/analysis.cpp
    ../src/parse/ast.cpp
    ../src/gen/generator.cpp
    ../src/gen/x86_64.cpp
    ../src/gen/x86_64astgen.cpp
    ../src/gen/castgen.cpp
)

add_executable(Frost ${SOURCES})