#include "cm/net/TcpServer.hpp"
#include "cm/net/EventLoop.hpp"
#include "cm/base/Log.hpp"

class EchoServer {
public:
	EchoServer(cm::net::EventLoop *loop,
	           const cm::net::InetAddress &addr,
	           const std::string &name)
			: server_(loop, addr, name) {
		// 注册回调函数
		server_.setConnectionCallback([](const cm::net::TcpConnectionPtr &conn) {
			if (conn->connected()) {
				LOG_INFO("Connection UP : %s", conn->peerAddress().toIpPort().c_str());
			} else {
				LOG_INFO("Connection DOWN : %s", conn->peerAddress().toIpPort().c_str());
			}
		});

		server_.setMessageCallback([](const cm::net::TcpConnectionPtr &conn,
		                              cm::net::Buffer *buf,
		                              cm::Timestamp time) {
			std::string msg = buf->retrieveAllAsString();
			std::cout << "===============read:" << msg << std::endl;
			conn->send(msg);
			conn->shutdown(); // 写端   EPOLLHUP =》 closeCallback_
		});

		server_.setThreadInitCallback([](cm::net::EventLoop *loop) {
			std::cout << "---------------setThreadInitCallback---------------" << std::endl;
		});

		server_.setWriteCompleteCallback([](const cm::net::TcpConnectionPtr &conn) {
			std::cout << "---------------setWriteCompleteCallback---------------" << std::endl;
		});
		// 设置合适的loop线程数量 loopthread
		server_.setThreadNum(3);
	}

	void start() {
		server_.start();
	}

private:
	cm::net::TcpServer server_;
};

int netTest() {
	cm::net::EventLoop loop;
	cm::net::InetAddress addr(8181);
	EchoServer server(&loop, addr, "EchoServer-01"); // Acceptor non-blocking listenfd  create bind
	server.start(); // listen  loopthread  listenfd => acceptChannel => mainLoop =>
	loop.loop(); // 启动mainLoop的底层Poller
	return 0;
}