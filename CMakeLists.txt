cmake_minimum_required(VERSION 3.16)
project(ClusterChat)
set(CMAKE_CXX_STANDARD 14)
file(GLOB SRC_FILES
        "${PROJECT_SOURCE_DIR}/src/server/*.h"
        "${PROJECT_SOURCE_DIR}/src/server/*.cpp"
        "${PROJECT_SOURCE_DIR}/src/server/db/*.h"
        "${PROJECT_SOURCE_DIR}/src/server/db/*.cpp")

link_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/libs
)
include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(${CMAKE_PROJECT_NAME} ${SRC_FILES})

target_link_libraries(ClusterChat
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/libmuduo_net.a
        libmuduo_http.a
        libmuduo_inspect.a
        libmuduo_base.a
        mysqlclient
        pthread)