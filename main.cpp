#include "chatServer.hpp"
#include "cm/net/EventLoop.hpp"

int main() {
	cm::net::EventLoop loop;
	cm::net::InetAddress addr(8181);
	ChatServer server(&loop, addr, "chatServer");
	server.start();
	loop.loop();
	return 0;
}