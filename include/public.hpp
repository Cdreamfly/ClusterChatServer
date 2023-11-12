#pragma once

enum class EnMsgType {
	LOGIN_MSG = 1,        // 登录消息
	LOGIN_MSG_ACK = 2,    // 登录响应消息
	LOGIN_OUT_MSG = 3,    // 注销消息
	REG_MSG = 4,          // 注册消息
	REG_MSG_ACK = 5,      // 注册响应消息
	ONE_CHAT_MSG = 6,     // 聊天消息
	ADD_FRIEND_MSG = 7,   // 添加好友消息
	CREATE_GROUP_MSG = 8, // 创建群组
	ADD_GROUP_MSG = 9,    // 加入群组
	GROUP_CHAT_MSG = 10,  // 群聊天
};

enum class ErrCode {
	SUCCESS = 0,    //正确
	FAILURE = 1,    //失败
	ONLINE = 2      //已在线
};
namespace Agreement {
	constexpr const char ID[] = "id";
	constexpr const char NAME[] = "name";
	constexpr const char STATE[] = "state";
	constexpr const char PASSWORD[] = "pwd";
	constexpr const char ONLINE[] = "online";
	constexpr const char OFFLINE[] = "offline";
	constexpr const char MSG_ID[] = "msgId";
	constexpr const char ERRNO[] = "errno";
	constexpr const char ERRMSG[] = "errmsg";
	constexpr const char OFFLINE_MSG[] = "offlineMsg";
	constexpr const char FRIENDS[] = "friends";
	constexpr const char FRIEND_ID[] = "friendId";
	constexpr const char GROUP_NAME[] = "groupName";
	constexpr const char GROUP_DESC[] = "groupDesc";
	constexpr const char GROUP_ID[] = "groupId";
	constexpr const char CREATOR[] = "creator";
	constexpr const char NORMAL[] = "normal";
	constexpr const char ROLE[] = "role";
	constexpr const char USERS[] = "users";
	constexpr const char GROUPS[] = "groups";
	constexpr const char TO_ID[] = "toId";

}