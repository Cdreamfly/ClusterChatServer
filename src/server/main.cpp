#include "server/chatserver.h"

int main() {
    muduo::net::EventLoop loop;
    muduo::net::InetAddress addr("127.0.0.1",9190);
    ChatServer server(&loop,addr,"ChatServer");
    server.Start();
    loop.loop();
    return 0;
}
