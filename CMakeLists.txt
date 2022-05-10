cmake_minimum_required(VERSION 3.16)
project(ClusterChat)

include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

link_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/libs
)

add_executable(ClusterChat src/main.cpp)
