cmake_minimum_required(VERSION 3.14)

project(MinCleanArchitecture
    VERSION 1.0.0
    LANGUAGES CXX
)

set(LIBRARY_TARGET_NAME architecture)
set(APPLICATION_TARGET_NAME example)

add_subdirectory("architecture")
add_subdirectory("example")