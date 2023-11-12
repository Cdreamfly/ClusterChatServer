cmake_minimum_required(VERSION 3.22.1)

project(ClusterChatServer)

set(CMAKE_CXX_STANDARD 17)

add_definitions("-g -Os -Wall -fPIC")

file(GLOB SRC_FILES
        "${PROJECT_SOURCE_DIR}/src/*.cpp")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

link_directories(${PROJECT_SOURCE_DIR}/lib/cm/)
link_directories(${PROJECT_SOURCE_DIR}/lib/mariadb/)
link_directories(${PROJECT_SOURCE_DIR}/lib/mariadb/plugin/)

add_executable(${CMAKE_PROJECT_NAME} main.cpp ${SRC_FILES})

target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC cm mariadbcpp)

find_package(Threads)