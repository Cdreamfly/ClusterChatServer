//
// Created by Cmf on 2022/5/11.
//
#include "server/chatservice.h"
#include "muduo/base/Logging.h"

ChatService &ChatService::Instance()
{
    static ChatService service;
    return service;
}
/*
 * 注册消息及对应的业务处理
 */
ChatService::ChatService()
{
    //用户基本业务管理相关事件处理回调注册
    _msgHandlerMap.insert({LOGIN_MSG, std::bind(&ChatService::Login, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)});
    _msgHandlerMap.insert({REG_MSG, std::bind(&ChatService::Reg, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)});
    _msgHandlerMap.insert({ONE_CHAT_MSG, std::bind(&ChatService::oneChat, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)});
    _msgHandlerMap.insert({ADD_FRIEND_MSG, std::bind(&ChatService::addFriend, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)});

}
/*
 * 登录
 */
void ChatService::Login(const muduo::net::TcpConnectionPtr &conn, json &js, muduo::Timestamp timestamp)
{
    int id = js["id"].get<int>();
    std::string pwd = js["pwd"];
    User user = _userModel.Query(id);
    //如果id和密码正确
    if(user.getId() == id && user.getPwd() == pwd)
    {
        //如果已经是登录状态
        if(user.getState() == "online")
        {
            json res;
            res["msgid"] = LOGIN_MSG_ACK;
            res["errno"] = 2;
            res["errmsg"] = "this account is using, input another!";
            conn->send(res.dump());
        }
        else
        {
            //不是则登录
            {
                //记录用户连接信息，要考虑线程安全，因为多线程访问
                std::lock_guard<std::mutex> lk(_mtx);
                _userConnMap.insert({id, conn});
            }
            //更新用户状态信息 state offline => online
            user.setState("online");
            _userModel.UpdateState(user);
            json res;
            res["msg-id"] = LOGIN_MSG_ACK;
            res["errno"] = 0;
            res["id"] = user.getId();
            res["name"] = user.getName();
            //查询是否有离线消息
            std::vector<std::string>vec = _offlineMsgModel.Query(id);
            if(!vec.empty())
            {
                res["offlinemsg"] = vec;
                _offlineMsgModel.Remove(id);
            }
            //查询是否有好友
            std::vector<User>userVec = _friendModel.Query(id);
            if(!userVec.empty())
            {
                std::vector<std::string>friendVec;
                for(auto& it:userVec)
                {
                    json js;
                    js["id"] = it.getId();
                    js["name"] = it.getName();
                    js["state"] = it.getState();
                    friendVec.emplace_back(js.dump());
                }
                res["friends"] = friendVec;
            }
            conn->send(res.dump());
        }
    }
    else
    {
        //不正确或找不到的返回
        json res;
        res["msgid"] = LOGIN_MSG_ACK;
        res["errno"] = 1;
        res["errmsg"] = "id or password is invalid!";
        conn->send(res.dump());
    }
}
/*
 * 注册
 */
void ChatService::Reg(const muduo::net::TcpConnectionPtr &conn, json &js, muduo::Timestamp timestamp)
{
    //获取客户端的注册信息创建对象
    User user(-1,js["name"],js["pwd"]);
    if(_userModel.Insert(user))
    {
        //插入新用户成功
        json res;
        res["msgid"] = REG_MSG_ACK;
        res["errno"] = 0;
        res["id"] = user.getId();
        conn->send(res.dump());
    }
    else
    {
        //插入新用户失败
        json res;
        res["msgid"] = REG_MSG_ACK;
        res["errno"] = 1;
        res["id"] = user.getId();
        conn->send(res.dump());
    }
}
/*
 * 私聊
 */
void ChatService::oneChat(const muduo::net::TcpConnectionPtr &conn, json &js, muduo::Timestamp timestamp)
{
    int toid = js["toid"].get<int>();
    {
        std::lock_guard<std::mutex> lk(_mtx);
        auto it = _userConnMap.find(toid);
        if(it != _userConnMap.end())
        {
            //如果在线就发送消息
            it->second->send(js.dump());
        }
    }
    //不在线就存储离线消息
    _offlineMsgModel.Install(toid, js.dump());
}
/*
 * 添加好友
 */
void ChatService::addFriend(const muduo::net::TcpConnectionPtr &conn, json &js, muduo::Timestamp timestamp)
{
    int userId = js["id"].get<int>();
    int friendId = js["friendid"].get<int>();
    _friendModel.Insert(userId,friendId);
}
/*
 * 获取消息id对应的处理函数
 */
MsgHandler ChatService::GetHandler(int msgId)
{
    auto it = _msgHandlerMap.find(msgId);
    if(it != _msgHandlerMap.end())
    {
        //如果找到了消息id对应的处理函数就把函数返回
        return _msgHandlerMap[msgId];
    }
    else
    {
        //没找到就返回一个空操作的处理函数
        return [=](const muduo::net::TcpConnectionPtr &conn, json &js, muduo::Timestamp timestamp)
        {
            LOG_ERROR<<"MsgId:"<<msgId<<" Can not find handler!";
        };
    }
}
ChatService::~ChatService()
{

}
/*
 * 找到异常退出的链接删了它并设置为离线状态
 */
void ChatService::clientCloseException(const muduo::net::TcpConnectionPtr &conn)
{
    User user;
    {
        std::lock_guard<std::mutex>lk(_mtx);
        for(auto it = _userConnMap.begin(); it != _userConnMap.end(); ++it)
        {
            if(it->second == conn)
            {
                user.setId(it->first);
                user.setState("offline");
                _userConnMap.erase(it);
                break;
            }
        }
    }
    if(user.getId() != -1)
    {
        _userModel.UpdateState(user);
    }
}
/*
 * //把online状态的用户，设置成offline
 */
void ChatService::Reset()
{
    _userModel.ReState();
}
