cmake_minimum_required(VERSION 3.31)

project(assessment)

set(CMAKE_CXX_STANDARD 20)

add_library(core INTERFACE)

target_include_directories(core INTERFACE core)

add_library(feature INTERFACE)

target_include_directories(feature INTERFACE feature)

file(GLOB IO_SOURCES io/*.cpp)

add_library(io STATIC ${IO_SOURCES})

target_include_directories(io PUBLIC io)

add_executable(${PROJECT_NAME}
        src/simulation.cpp
        src/main.cpp)

target_link_libraries(${PROJECT_NAME} PUBLIC core feature io)