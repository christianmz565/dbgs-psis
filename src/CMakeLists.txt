cmake_minimum_required(VERSION 3.10)
project(DBGS CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

add_executable(TableTest TableTest.cpp)
add_executable(ParsingTest ParsingTest.cpp)
add_executable(StressTest StressTest.cpp)
add_executable(BulkTest BulkTest.cpp)
add_executable(Main Main.cpp)

target_include_directories(TableTest PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(ParsingTest PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(StressTest PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(BulkTest PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(Main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(TableTest PRIVATE
        #-Wall
        #-Wextra
        #-pedantic
        -g
        -O2
    )
    target_compile_options(ParsingTest PRIVATE
        #-Wall
        #-Wextra
        #-pedantic
        -g
        -O2
    )
    target_compile_options(StressTest PRIVATE
        #-Wall
        #-Wextra
        #-pedantic
        -g
        -O2
    )
    target_compile_options(BulkTest PRIVATE
        #-Wall
        #-Wextra
        #-pedantic
        -g
        -O2
    )
    target_compile_options(Main PRIVATE
        -Wall
        -Wextra
        -pedantic
        -g
        -O2
    )
endif()