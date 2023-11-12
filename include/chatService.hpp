#pragma once

#include <unordered_map>
#include <functional>

#include "json.hpp"
#include "public.hpp"
#include "cm/net/TcpConnection.hpp"

using json = nlohmann::json;

using MsgHandler = std::function<void(const cm::net::TcpConnectionPtr &, const json &, cm::Timestamp)>;

class ChatService {
public:
	static ChatService &getInstance();

	MsgHandler getHandler(EnMsgType);

	~ChatService() = default;

private:
	ChatService();


private:
	std::unordered_map<EnMsgType, MsgHandler> msgHandlerMap_;  //一个消息id对应一个处理函数
};



