cmake_minimum_required(VERSION 3.13)

project(turing)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

add_compile_options(-fsanitize=address)
add_link_options(-fsanitize=address)

include_directories(turing-project/src)

add_executable(${CMAKE_PROJECT_NAME} turing-project/src/main.cpp
    turing-project/src/turing/cli/cli.cpp
    turing-project/src/turing/machine/direction.cpp
    turing-project/src/turing/machine/machine.cpp
    turing-project/src/turing/parser/statement_parser.cpp
    turing-project/src/turing/util/file.cpp
    turing-project/src/turing/util/string.cpp
)

add_executable(test_statement_parser turing-project/test/turing/parser/statement_parser_test.cpp
    turing-project/src/turing/machine/direction.cpp
    turing-project/src/turing/parser/statement_parser.cpp
    turing-project/src/turing/util/file.cpp
    turing-project/src/turing/util/string.cpp
)