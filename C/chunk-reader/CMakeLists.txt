cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

project(chunk-reader
        VERSION
        0.1
        DESCRIPTION
        "A simple chunk reader to play with Rust FFI"
        LANGUAGES
        C
        )

set(CMAKE_C_FLAGS "-Iinclude -fPIC -Wall")

add_library(chunk-reader
        STATIC
        src/chunk-reader.c
        include/chunk-reader.h
        )
target_include_directories(chunk-reader PUBLIC include)
set_target_properties(chunk-reader
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "./lib"
        LIBRARY_OUTPUT_DIRECTORY "./lib"
        RUNTIME_OUTPUT_DIRECTORY "./bin"
        )
target_link_libraries(chunk-reader -lc)

add_executable(main
        src/main.c
        )
target_link_libraries(main chunk-reader)
