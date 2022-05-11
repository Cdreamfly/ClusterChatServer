cmake_minimum_required(VERSION 3.16)
project(test_muduo)

link_directories(../../libs)
include_directories(../../include)

add_executable(test_muduo test_muduo.cpp)

target_link_libraries(${CMAKE_PROJECT_NAME}
        libmuduo_net.a
        libmuduo_http.a
        libmuduo_inspect.a
        libmuduo_base.a
        pthread)