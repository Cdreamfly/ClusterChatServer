#pragma once

#include <unordered_map>
#include <functional>
#include <mutex>

#include "json.hpp"
#include "public.hpp"
#include "cm/net/TcpConnection.hpp"
#include "model/userModel.hpp"

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
	std::unordered_map<long, cm::net::TcpConnectionPtr> userConnMap_;  //存储在线用户连接信息
	std::mutex mtx_;
	UserModel userModel_;
};



