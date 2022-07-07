cmake_minimum_required(VERSION 3.13.0)
project(mtfind)

set(CMAKE_CXX_STANDARD 17)

file(GLOB SOURCES ${PROJECT_SOURCE_DIR} "*.cpp" "*.hpp")

add_executable(mtfind ${SOURCES})
