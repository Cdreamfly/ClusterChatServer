cmake_minimum_required(VERSION 3.22.1)

project(ClusterChatServer)

set(CMAKE_CXX_STANDARD 17)

add_definitions("-g -Os -Wall -fPIC")

add_executable(${CMAKE_PROJECT_NAME} main.cpp)

target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/lib/libcm.so)

find_package(Threads)