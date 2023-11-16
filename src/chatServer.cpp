#include "chatServer.hpp"
#include "cm/base/Log.hpp"
#include "chatService.hpp"

ChatServer::ChatServer(cm::net::EventLoop *loop, const cm::net::InetAddress &addr, const std::string_view &name)
		: server_(loop, addr, name.data()) {
	server_.setConnectionCallback([&](const cm::net::TcpConnectionPtr &conn) {
		if (!conn->connected()) {
			conn->shutdown();
		}
	});

	server_.setMessageCallback([&](const cm::net::TcpConnectionPtr &conn,
	                               cm::net::Buffer *buf,
	                               const cm::Timestamp &time) {
		json js;
		EnMsgType type;
		try {
			js = json::parse(buf->retrieveAllAsString());
		}
		catch (const json::exception &e) {
			std::cout << e.what() << std::endl;
			conn->send("json parse error! " + time.toString());
			return;
		}
		try {
			type = js.at(Agreement::MSG_ID).get<EnMsgType>();
		}
		catch (const json::exception &e) {
			std::cout << e.what() << std::endl;
		}
		ChatService::getInstance().getHandler(type)(conn, js, time);
	});

	server_.setThreadNum(3);
}

void ChatServer::start() {
	server_.start();
}