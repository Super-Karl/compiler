cmake_minimum_required(VERSION 3.16)
project(compiler VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)

find_package(BISON)
find_package(FLEX)
make_directory(${CMAKE_CURRENT_BINARY_DIR}/front)
#BISON_TARGET(Bison src/front/bison.y)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/front)
include_directories(src)

file(GLOB_RECURSE srcs src/*.cpp)

add_executable(compiler
        ${srcs}
        )
