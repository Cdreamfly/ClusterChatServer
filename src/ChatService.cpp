#include "chatService.hpp"
#include "cm/base/Log.hpp"
#include "model/user.hpp"

ChatService::ChatService() {
	msgHandlerMap_.insert({
		EnMsgType::LOGIN_MSG, [&](const cm::net::TcpConnectionPtr &conn,
		                          const json &js,
		                          const cm::Timestamp timestamp) {
			long id = js.at(Agreement::ID).get<long>();
			const User user = UserModel::query(id);
			std::cout << user.getId() << user.getName() << std::endl;
			if (user.getId() == 0) {
				json res;
				res[Agreement::MSG_ID] = EnMsgType::LOGIN_MSG_ACK;
				res[Agreement::ERRNO] = ErrCode::NOUSER;
				res[Agreement::ERRMSG] = "id not registered!";
				conn->send(res.dump());
			} else if (user.getId() == id && user.getPwd() == js.at(Agreement::PASSWORD)) {
				if (user.getState() == Agreement::ONLINE) {
					json res;
					res[Agreement::MSG_ID] = EnMsgType::LOGIN_MSG_ACK;
					res[Agreement::ERRNO] = ErrCode::ONLINE;
					res[Agreement::ERRMSG] = "this account is using, input another!";
					conn->send(res.dump());
				} else {
					{
						std::lock_guard<std::mutex> lock{mtx_};
						userConnMap_.insert({id, conn});
					}
				}
			} else {
				json res;
				res[Agreement::MSG_ID] = EnMsgType::LOGIN_MSG_ACK;
				res[Agreement::ERRNO] = ErrCode::FAILURE;
				res[Agreement::ERRMSG] = "id or password is invalid!";
				conn->send(res.dump());
			}
		}
	});

	msgHandlerMap_.insert({
		EnMsgType::REG_MSG, [&](const cm::net::TcpConnectionPtr &conn,
		                        const json &js,
		                        const cm::Timestamp timestamp) {
			if (User user(-1, js.at(Agreement::NAME).get<std::string_view>(),
			              js.at(Agreement::PASSWORD).get<std::string_view>()); UserModel::insert(user)) {
				json res;
				res[Agreement::MSG_ID] = EnMsgType::REG_MSG_ACK;
				res[Agreement::ERRNO] = ErrCode::SUCCESS;
				res[Agreement::ID] = user.getId();
				conn->send(res.dump());
			} else {
				json res;
				res[Agreement::MSG_ID] = EnMsgType::REG_MSG_ACK;
				res[Agreement::ERRNO] = ErrCode::FAILURE;
				res[Agreement::ID] = user.getId();
				conn->send(res.dump());
			}
		}
	});
}

ChatService &ChatService::getInstance() {
	static ChatService chatService;
	return chatService;
}

MsgHandler ChatService::getHandler(const EnMsgType msgId) {
	if (msgHandlerMap_.find(msgId) != msgHandlerMap_.end()) {
		return msgHandlerMap_[msgId];
	}
	return [&](const cm::net::TcpConnectionPtr &connection, const json &js, const cm::Timestamp timestamp) {
		LOG_ERROR("MsgId Can not find handler!", js.at(Agreement::MSG_ID).get<int>());
		const std::string res = "MsgId:" + std::to_string(js.at(Agreement::MSG_ID).get<int>()) + " Can not find!";
		connection->send(res + " " + timestamp.toString());
	};
}
