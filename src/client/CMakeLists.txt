include_directories(../../include)
#指定生成可执行文件
add_executable(ChatClient client.cpp ../server/model/user.cpp ../server/model/group.cpp ../server/model/groupuser.cpp)
#指定可执行文件链接时需要依赖的库文件
target_link_libraries(ChatClient pthread)


