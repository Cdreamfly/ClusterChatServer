cmake_minimum_required(VERSION 3.16)
project(ClusterChatServer)
set(CMAKE_CXX_STANDARD 14)

file(GLOB SRC_FILES
        "${PROJECT_SOURCE_DIR}/src/server/*.h"
        "${PROJECT_SOURCE_DIR}/src/server/*.cpp"
        "${PROJECT_SOURCE_DIR}/src/server/db/*.h"
        "${PROJECT_SOURCE_DIR}/src/server/db/*.cpp"
        "${PROJECT_SOURCE_DIR}/src/server/model/*.h"
        "${PROJECT_SOURCE_DIR}/src/server/model/*.cpp")

link_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/libs
)
include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include/CmfNet
)

add_executable(${CMAKE_PROJECT_NAME} ${SRC_FILES})

target_link_libraries(ClusterChatServer
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/libCmfNet.a
        mysqlclient
        hiredis
        pthread)
#加载子目录
#add_subdirectory(src/client)
