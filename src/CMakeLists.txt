cmake_minimum_required(VERSION 3.10)
project(DBGS CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

add_executable(DBGS Main.cpp)

target_include_directories(DBGS PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(DBGS PRIVATE
        #-Wall
        #-Wextra
        #-pedantic
        -g
        -O2
    )
endif()