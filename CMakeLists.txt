cmake_minimum_required(VERSION 4.0)

project(BorderlessASI LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(BorderlessASI SHARED
        src/main.cpp
)

target_compile_definitions(BorderlessASI PRIVATE
        UNICODE
        _UNICODE
        WIN32_LEAN_AND_MEAN
)

target_link_libraries(BorderlessASI PRIVATE
        user32
)

set_target_properties(BorderlessASI PROPERTIES
        PREFIX ""
        SUFFIX ".asi"
)