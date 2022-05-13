#include "server/chatserver.h"
#include "server/chatservice.h"
#include <signal.h>

//处理服务器ctrl+c结束后，进行重置user的状态信息
void resetState(int)
{
    ChatService::Instance().Reset();
    exit(0);
}

int main() {

    struct sigaction sa;
    sa.sa_handler = resetState;
    sa.sa_flags = 0;
    sigaction(SIGINT,&sa, nullptr);

    muduo::net::EventLoop loop;
    muduo::net::InetAddress addr("127.0.0.1",9190);
    ChatServer server(&loop,addr,"ChatServer");
    server.Start();
    loop.loop();
    return 0;
}
