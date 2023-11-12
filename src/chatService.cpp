#include "chatService.hpp"
#include "cm/base/Log.hpp"

ChatService::ChatService() {
	msgHandlerMap_.insert({EnMsgType::LOGIN_MSG, [&](const cm::net::TcpConnectionPtr &connection,
	                                                 const json &js,
	                                                 const cm::Timestamp timestamp) {
		connection->send(js.dump() + timestamp.toString());
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
			LOG_ERROR("MsgId: %d Can not find handler!", static_cast<int>(msgId));
		};
	}
}

