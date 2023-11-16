#include "chatService.hpp"
#include "cm/base/Log.hpp"

ChatService::ChatService() {
	msgHandlerMap_.insert({EnMsgType::LOGIN_MSG, [&](const cm::net::TcpConnectionPtr &connection,
	                                                 const json &js,
	                                                 const cm::Timestamp timestamp) {
		connection->send(js.dump() + timestamp.toString());
	}});

	msgHandlerMap_.insert({EnMsgType::REG_MSG, [&](const cm::net::TcpConnectionPtr &connection,
	                                               const json &js,
	                                               const cm::Timestamp timestamp) {

	}});
}

ChatService &ChatService::getInstance() {
	static ChatService chatService;
	return chatService;
}

MsgHandler ChatService::getHandler(const EnMsgType msgId) {
	if (msgHandlerMap_.find(msgId) != msgHandlerMap_.end()) {
		return msgHandlerMap_[msgId];
	} else {
		return [&](const cm::net::TcpConnectionPtr &connection,
		           const json &js,
		           const cm::Timestamp timestamp) {
			LOG_ERROR("MsgId Can not find handler!", js.at(Agreement::MSG_ID).get<int>());
			std::string res = "MsgId:" + std::to_string(js.at(Agreement::MSG_ID).get<int>()) + " Can not find!";
			connection->send(res + " " + timestamp.toString());
		};
	}
}

