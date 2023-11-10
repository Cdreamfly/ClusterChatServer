cmake_minimum_required(VERSION 3.22.1)

project(ClusterChatServer)

set(CMAKE_CXX_STANDARD 17)

add_definitions("-g -Os -Wall -fPIC")

link_directories(${PROJECT_SOURCE_DIR}/lib/cm/)
link_directories(${PROJECT_SOURCE_DIR}/lib/mariadb/)
link_directories(${PROJECT_SOURCE_DIR}/lib/mariadb/plugin/)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(${CMAKE_PROJECT_NAME} main.cpp)

target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC cm mariadbcpp)

find_package(Threads)