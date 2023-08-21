cmake_minimum_required(VERSION 3.21)

project(TestDrivenDevelopment
    DESCRIPTION "Test Driven Development in C++"
    LANGUAGES CXX C
)

include(cmake/setup.cmake)
include(cmake/findDependencies.cmake)
include(cmake/compilerFlags.cmake)

add_subdirectory(c2)

include(cmake/summary.cmake)