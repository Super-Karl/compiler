cmake_minimum_required(VERSION 3.16)
project(compiler VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)

find_package(BISON)
find_package(FLEX)
make_directory(${CMAKE_CURRENT_BINARY_DIR}/front)
BISON_TARGET(Parser src/front/parser/bison.y ${CMAKE_CURRENT_BINARY_DIR}/front/grammar.cpp)
FLEX_TARGET(Scanner src/front/parser/front_flex.l ${CMAKE_CURRENT_BINARY_DIR}}/front/tokens.cpp)
ADD_FLEX_BISON_DEPENDENCY(Scanner Parser)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/front)
include_directories(src)


file(GLOB_RECURSE srcs src/*.cpp)

add_executable(compiler
        ${srcs}
#        ${BISON_Parser_OUTPUTS}
#        ${FLEX_Scanner_OUTPUTS}
       )
