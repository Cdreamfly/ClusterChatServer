//
// Created by Cmf on 2022/5/11.
//

#ifndef CLUSTERCHAT_CHATSERVICE_H
#define CLUSTERCHAT_CHATSERVICE_H

#include <unordered_map>
#include <functional>
#include <mutex>
#include "CmfNet/net/TcpConnection.hpp"
#include "server/model/usermodel.h"
#include "server/model/offlinemessagemodel.h"
#include "server/model/friendmodel.h"
#include "server/model/groupmodel.h"
#include "server/db/redis.h"
#include "json.hpp"
#include "public.h"

using json = nlohmann::json;
using MsgHandler = std::function<void(const TcpConnectionPtr &, json &, Timestamp)>;

/*
 * 业务模块
 */
class ChatService {
public:
    //获取单例对象接口函数
    static ChatService &Instance();

    //处理登录
    void Login(const TcpConnectionPtr &conn, json &js, Timestamp timestamp);

    //处理注册
    void Reg(const TcpConnectionPtr &conn, json &js, Timestamp timestamp);

    //一对一聊天
    void oneChat(const TcpConnectionPtr &conn, json &js, Timestamp timestamp);

    //添加好友
    void addFriend(const TcpConnectionPtr &conn, json &js, Timestamp timestamp);

    //创建群
    void createGroup(const TcpConnectionPtr &conn, json &js, Timestamp timestamp);

    //添加群
    void addGroup(const TcpConnectionPtr &conn, json &js, Timestamp timestamp);

    //群聊天
    void groupChat(const TcpConnectionPtr &conn, json &js, Timestamp timestamp);

    //注销
    void loginOut(const TcpConnectionPtr &conn, json &js, Timestamp timestamp);

    //获取消息id对应的处理函数
    MsgHandler GetHandler(EnMsgType msgId);

    //处理客户端异常推出
    void clientCloseException(const TcpConnectionPtr &conn);

    //从redis消息队列中获取订阅的消息
    void handleRedisSubscribeMessage(int, std::string);

    //服务器异常处理
    void Reset();

    ~ChatService() {}

private:
    ChatService();//单例

private:
    std::unordered_map<EnMsgType, MsgHandler> _msgHandlerMap;  //一个消息id对应一个处理函数
    std::unordered_map<int, TcpConnectionPtr> _userConnMap;  //存储在线用户连接信息
    std::mutex _mtx;
    UserModel _userModel;   //用户业务
    OfflineMsgModel _offlineMsgModel;   //离线消息业务
    FriendModel _friendModel;   //好友业务
    GroupModel _groupModel; //群业务
    Redis _redis;
};

#endif //CLUSTERCHAT_CHATSERVICE_H