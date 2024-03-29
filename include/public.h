//
// Created by Cmf on 2022/5/11.
//

#ifndef CLUSTERCHAT_PUBLIC_H
#define CLUSTERCHAT_PUBLIC_H

enum class EnMsgType : int {
    LOGIN_MSG = 1,  // 登录消息
    LOGIN_MSG_ACK = 2,  // 登录响应消息
    LOGIN_OUT_MSG = 3,   // 注销消息
    REG_MSG = 4,        // 注册消息
    REG_MSG_ACK = 5,    // 注册响应消息
    ONE_CHAT_MSG = 6,   // 聊天消息
    ADD_FRIEND_MSG = 7, // 添加好友消息
    CREATE_GROUP_MSG = 8, // 创建群组
    ADD_GROUP_MSG = 9,    // 加入群组
    GROUP_CHAT_MSG = 10,   // 群聊天
};

enum class ErrCode : int {
    SUCCESS = 0,    //正确
    FAILURE = 1,    //失败
    ONLINE = 2      //已在线
};

namespace Agreement {
    const char *const ID = "id";
    const char *const NAME = "name";
    const char *const STATE = "state";
    const char *const PASSWORD = "pwd";
    const char *const ONLINE = "online";
    const char *const OFFLINE = "offline";
    const char *const MSG_ID = "msgid";
    const char *const ERRNO = "errno";
    const char *const ERRMSG = "errmsg";
    const char *const OFFLINE_MSG = "offlinemsg";
    const char *const FRIENDS = "friends";
    const char *const FRIEND_ID = "friendid";
    const char *const GROUP_NAME = "groupname";
    const char *const GROUP_DESC = "groupdesc";
    const char *const GROUP_ID = "groupid";
    const char *const CREATOR = "creator";
    const char *const NORMAL = "normal";
    const char *const ROLE = "role";
    const char *const USERS = "users";
    const char *const GROUPS = "groups";
    const char *const TO_ID = "toid";
}

#endif //CLUSTERCHAT_PUBLIC_H