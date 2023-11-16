#pragma once

#include "cm/net/TcpServer.hpp"

class ChatServer {
public:
	ChatServer(cm::net::EventLoop *, const cm::net::InetAddress &, const std::string_view &);

	void start();

private:
	cm::net::TcpServer server_;
};

