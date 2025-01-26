cmake_minimum_required(VERSION 3.26)
project(magic_vtable)

add_library(magic_vtable INTERFACE)
target_include_directories(magic_vtable INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_compile_features(magic_vtable INTERFACE cxx_std_17)

option(BUILD_MVFT_TESTS "Build tests" OFF)
if(BUILD_MVFT_TESTS)
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/test)
endif ()
