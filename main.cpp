#include "chatServer.hpp"
#include "cm/net/EventLoop.hpp"

int main() {
	cm::net::EventLoop loop;
	cm::net::InetAddress addr(8181);
	ChatServer server(&loop, addr, "chat"); // Acceptor non-blocking listenfd  create bind
	server.start(); // listen  loopthread  listenfd => acceptChannel => mainLoop =>
	loop.loop();
}