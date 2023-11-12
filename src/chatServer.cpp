#include "chatServer.hpp"
#include "cm/base/Log.hpp"
#include "chatService.hpp"

ChatServer::ChatServer(cm::net::EventLoop *loop, const cm::net::InetAddress &addr, const std::string &name)
		: server_(loop, addr, name) {
	server_.setConnectionCallback([&](const cm::net::TcpConnectionPtr &conn) {
		if (!conn->connected()) {
			conn->shutdown();
		}
	});

	server_.setMessageCallback([&](const cm::net::TcpConnectionPtr &conn,
	                               cm::net::Buffer *buf,
	                               const cm::Timestamp &time) {
		json js = json::parse(buf->retrieveAllAsString());
		ChatService::getInstance().getHandler(js[Agreement::MSG_ID].get<EnMsgType>())(conn, js, time);
	});

	server_.setThreadNum(3);
}

void ChatServer::start() {
	server_.start();
}