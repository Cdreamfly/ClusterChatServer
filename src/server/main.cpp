#include "server/chatserver.h"
#include "server/db/db.h"

#include <iostream>


int main() {
    MySQL sql;
    std::cout<< sql.Connect()<<" hello"<<std::endl;
    muduo::net::EventLoop loop;
    muduo::net::InetAddress addr("127.0.0.1",9190);
    ChatServer server(&loop,addr,"ChatServer");
    server.Start();
    loop.loop();
    return 0;
}
