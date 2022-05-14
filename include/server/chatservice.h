//
// Created by Cmf on 2022/5/11.
//

#ifndef CLUSTERCHAT_CHATSERVICE_H
#define CLUSTERCHAT_CHATSERVICE_H

#include <unordered_map>
#include <functional>
#include <mutex>
#include "muduo/net/TcpConnection.h"
#include "json.hpp"
#include "public.h"
#include "server/model/usermodel.h"
#include "server/model/offlinemessagemodel.h"
#include "server/model/friendmodel.h"
#include "server/model/groupmodel.h"

using json = nlohmann::json;
using MsgHandler = std::function<void(const muduo::net::TcpConnectionPtr&,json&, muduo::Timestamp)>;

/*
 * 业务模块
 */
class ChatService
{
public:
    //获取单例对象接口函数
    static ChatService&Instance();
    //处理登录
    void Login(const muduo::net::TcpConnectionPtr&conn,json&js, muduo::Timestamp timestamp);
    //处理注册
    void Reg(const muduo::net::TcpConnectionPtr&conn,json&js, muduo::Timestamp timestamp);
    //一对一聊天
    void oneChat(const muduo::net::TcpConnectionPtr&conn,json&js, muduo::Timestamp timestamp);
    //添加好友
    void addFriend(const muduo::net::TcpConnectionPtr&conn,json&js, muduo::Timestamp timestamp);
    //创建群
    void createGroup(const muduo::net::TcpConnectionPtr&conn,json&js, muduo::Timestamp timestamp);
    //添加群
    void addGroup(const muduo::net::TcpConnectionPtr&conn,json&js, muduo::Timestamp timestamp);
    //群聊天
    void groupChat(const muduo::net::TcpConnectionPtr&conn,json&js, muduo::Timestamp timestamp);

    //获取消息id对应的处理函数
    MsgHandler GetHandler(int msgId);
    //处理客户端异常推出
    void clientCloseException(const muduo::net::TcpConnectionPtr& conn);
    //服务器异常处理
    void Reset();
    ~ChatService();

private:
    ChatService();//单例

private:
    std::unordered_map<int,MsgHandler> _msgHandlerMap;  //一个消息id对应一个处理函数
    std::unordered_map<int,muduo::net::TcpConnectionPtr> _userConnMap;  //存储在线用户连接信息
    std::mutex _mtx;
    UserModel _userModel;   //用户业务
    OfflineMsgModel _offlineMsgModel;   //离线消息业务
    FriendModel _friendModel;   //好友业务
    GroupModel _groupModel; //群业务
};

#endif //CLUSTERCHAT_CHATSERVICE_H
