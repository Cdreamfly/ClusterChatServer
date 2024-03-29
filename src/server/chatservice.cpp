//
// Created by Cmf on 2022/5/11.
//
#include "server/chatservice.h"
#include "CmfNet/base/Log.hpp"

ChatService &ChatService::Instance() {
    static ChatService service;
    return service;
}

/*
 * 注册消息及对应的业务处理
 */
ChatService::ChatService() {
    //用户基本业务管理相关事件处理回调注册
    _msgHandlerMap.insert({EnMsgType::LOGIN_MSG,
                           std::bind(&ChatService::Login, this, std::placeholders::_1, std::placeholders::_2,
                                     std::placeholders::_3)});
    _msgHandlerMap.insert({EnMsgType::REG_MSG,
                           std::bind(&ChatService::Reg, this, std::placeholders::_1, std::placeholders::_2,
                                     std::placeholders::_3)});
    _msgHandlerMap.insert({EnMsgType::ONE_CHAT_MSG,
                           std::bind(&ChatService::oneChat, this, std::placeholders::_1, std::placeholders::_2,
                                     std::placeholders::_3)});
    _msgHandlerMap.insert({EnMsgType::ADD_FRIEND_MSG,
                           std::bind(&ChatService::addFriend, this, std::placeholders::_1, std::placeholders::_2,
                                     std::placeholders::_3)});
    _msgHandlerMap.insert({EnMsgType::LOGIN_OUT_MSG,
                           std::bind(&ChatService::loginOut, this, std::placeholders::_1, std::placeholders::_2,
                                     std::placeholders::_3)});
    //群组业务管理相关事件处理回调注册
    _msgHandlerMap.insert({EnMsgType::CREATE_GROUP_MSG,
                           std::bind(&ChatService::createGroup, this, std::placeholders::_1, std::placeholders::_2,
                                     std::placeholders::_3)});
    _msgHandlerMap.insert({EnMsgType::ADD_GROUP_MSG,
                           std::bind(&ChatService::addGroup, this, std::placeholders::_1, std::placeholders::_2,
                                     std::placeholders::_3)});
    _msgHandlerMap.insert({EnMsgType::GROUP_CHAT_MSG,
                           std::bind(&ChatService::groupChat, this, std::placeholders::_1, std::placeholders::_2,
                                     std::placeholders::_3)});
    //连接redis服务器
    if (_redis.Connect()) {
        //设置上报消息的回调
        _redis.InitNotifyHandler(std::bind(&ChatService::handleRedisSubscribeMessage, this, std::placeholders::_1,
                                           std::placeholders::_2));
    }
}

/*
 * 登录
 */
void ChatService::Login(const TcpConnectionPtr &conn, json &js, Timestamp timestamp) {
    int id = js[Agreement::ID].get<int>();
    std::string pwd = js[Agreement::PASSWORD];
    User user = _userModel.Query(id);
    //如果id和密码正确
    if (user.getId() == id && user.getPwd() == pwd) {
        //如果已经是登录状态
        if (user.getState() == Agreement::ONLINE) {
            json res;
            res[Agreement::MSG_ID] = EnMsgType::LOGIN_MSG_ACK;
            res[Agreement::ERRNO] = ErrCode::ONLINE;
            res[Agreement::ERRMSG] = "this account is using, input another!";
            conn->Send(res.dump());
        } else {
            //不是则登录
            {
                //记录用户连接信息，要考虑线程安全，因为多线程访问
                std::lock_guard<std::mutex> lk(_mtx);
                _userConnMap.insert({id, conn});
            }
            //id用户登录成功后，向redis订阅channel(id)
            _redis.Subscribe(id);
            //更新用户状态信息 state offline => online
            user.setState(Agreement::ONLINE);
            _userModel.UpdateState(user);
            json res;
            res[Agreement::MSG_ID] = EnMsgType::LOGIN_MSG_ACK;
            res[Agreement::ERRNO] = ErrCode::SUCCESS;
            res[Agreement::ID] = user.getId();
            res[Agreement::NAME] = user.getName();
            //查询是否有离线消息
            std::vector<std::string> vec = _offlineMsgModel.Query(id);
            if (!vec.empty()) {
                res[Agreement::OFFLINE_MSG] = vec;
                _offlineMsgModel.Remove(id);
            }
            //查询是否有好友
            std::vector<User> userVec = _friendModel.Query(id);
            if (!userVec.empty()) {
                std::vector<std::string> friendVec;
                for (auto &it: userVec) {
                    json js;
                    js[Agreement::ID] = it.getId();
                    js[Agreement::NAME] = it.getName();
                    js[Agreement::STATE] = it.getState();
                    friendVec.emplace_back(js.dump());
                }
                res[Agreement::FRIENDS] = friendVec;
            }
            //查询用户是否有群
            std::vector<Group> groupUsrVec = _groupModel.queryGroup(id);
            if (!groupUsrVec.empty()) {
                //获取群的信息
                std::vector<std::string> groupVec;
                for (auto &group: groupUsrVec) {
                    json groupJson;
                    groupJson[Agreement::ID] = group.getId();
                    groupJson[Agreement::GROUP_NAME] = group.getName();
                    groupJson[Agreement::GROUP_DESC] = group.getDesc();
                    //获取每个群里成员的信息
                    std::vector<std::string> usrVec;
                    for (auto &it: group.getGroupUsers()) {
                        json userJson;
                        userJson[Agreement::ID] = it.getId();
                        userJson[Agreement::NAME] = it.getName();
                        userJson[Agreement::STATE] = it.getState();
                        userJson[Agreement::ROLE] = it.getRole();
                        usrVec.emplace_back(userJson.dump());
                    }
                    groupJson[Agreement::USERS] = usrVec;
                    groupVec.emplace_back(groupJson.dump());
                }
                res[Agreement::GROUPS] = groupVec;
            }
            conn->Send(res.dump());
        }
    } else {
        //不正确或找不到的返回
        json res;
        res[Agreement::MSG_ID] = EnMsgType::LOGIN_MSG_ACK;
        res[Agreement::ERRNO] = ErrCode::FAILURE;
        res[Agreement::ERRMSG] = "id or password is invalid!";
        conn->Send(res.dump());
    }
}

/*
 * 注册
 */
void ChatService::Reg(const TcpConnectionPtr &conn, json &js, Timestamp timestamp) {
    //获取客户端的注册信息创建对象
    User user(-1, js[Agreement::NAME], js[Agreement::PASSWORD]);
    if (_userModel.Insert(user)) {
        //插入新用户成功
        json res;
        res[Agreement::MSG_ID] = EnMsgType::REG_MSG_ACK;
        res[Agreement::ERRNO] = ErrCode::SUCCESS;
        res[Agreement::ID] = user.getId();
        conn->Send(res.dump());
    } else {
        //插入新用户失败
        json res;
        res[Agreement::MSG_ID] = EnMsgType::REG_MSG_ACK;
        res[Agreement::ERRNO] = ErrCode::FAILURE;
        res[Agreement::ID] = user.getId();
        conn->Send(res.dump());
    }
}

/*
 * 私聊
 */
void ChatService::oneChat(const TcpConnectionPtr &conn, json &js, Timestamp timestamp) {
    int toid = js[Agreement::TO_ID].get<int>();
    {
        std::lock_guard<std::mutex> lk(_mtx);
        auto it = _userConnMap.find(toid);
        if (it != _userConnMap.end()) {
            //如果在线就发送消息
            it->second->Send(js.dump());
            return;
        }
    }
    //跨服务器发送
    User user = _userModel.Query(toid);
    if (user.getState() == Agreement::ONLINE) {
        _redis.Publish(toid, js.dump());
        return;
    }
    //不在线就存储离线消息
    _offlineMsgModel.Insert(toid, js.dump());
}

/*
 * 添加好友
 */
void ChatService::addFriend(const TcpConnectionPtr &conn, json &js, Timestamp timestamp) {
    int userId = js[Agreement::ID].get<int>();
    int friendId = js[Agreement::FRIEND_ID].get<int>();
    _friendModel.Insert(userId, friendId);
}

void ChatService::createGroup(const TcpConnectionPtr &conn, json &js, Timestamp timestamp) {
    int userId = js[Agreement::ID].get<int>();
    Group group(-1, js[Agreement::GROUP_NAME], js[Agreement::GROUP_DESC]);
    if (_groupModel.createGroup(group)) {
        _groupModel.addGroup(userId, group.getId(), Agreement::CREATOR);
    }
}

void ChatService::addGroup(const TcpConnectionPtr &conn, json &js, Timestamp timestamp) {
    _groupModel.addGroup(js[Agreement::ID].get<int>(), js[Agreement::GROUP_ID].get<int>(), Agreement::NORMAL);
}

void ChatService::groupChat(const TcpConnectionPtr &conn, json &js, Timestamp timestamp) {
    //获取群里成员的id
    std::vector<int> idVec = _groupModel.queryGroupUsers(js[Agreement::ID].get<int>(),
                                                         js[Agreement::GROUP_ID].get<int>());
    for (int id: idVec) {
        auto it = _userConnMap.find(id);
        if (it != _userConnMap.end()) {
            //群里在线发送
            it->second->Send(js.dump());
        } else {
            //如果群里的人在不同服务器
            User user = _userModel.Query(id);
            if (user.getState() == Agreement::ONLINE) {
                _redis.Publish(id, js.dump());
            } else {
                //群里不在线离线存储
                _offlineMsgModel.Insert(id, js.dump());
            }
        }
    }
}

/*
 * 获取消息id对应的处理函数
 */
MsgHandler ChatService::GetHandler(EnMsgType msgId) {
    auto it = _msgHandlerMap.find(msgId);
    if (it != _msgHandlerMap.end()) {
        //如果找到了消息id对应的处理函数就把函数返回
        return _msgHandlerMap[msgId];
    } else {
        //没找到就返回一个空操作的处理函数
        return [=](const TcpConnectionPtr &conn, json &js, Timestamp timestamp) {
            LOG_ERROR("MsgId: %d Can not find handler!", static_cast<int>(msgId));
        };
    }
}

/*
 * 找到异常退出的链接删了它并设置为离线状态
 */
void ChatService::clientCloseException(const TcpConnectionPtr &conn) {
    User user;
    {
        std::lock_guard<std::mutex> lk(_mtx);
        for (auto it = _userConnMap.begin(); it != _userConnMap.end(); ++it) {
            if (it->second == conn) {
                user.setId(it->first);
                _userConnMap.erase(it);
                break;
            }
        }
    }
    //用户注销，相当于就是下线，在redis中取消订阅通道
    _redis.UnSubscribe(user.getId());
    if (user.getId() != -1) {
        user.setState(Agreement::OFFLINE);
        _userModel.UpdateState(user);
    }
}

/*
 * //把online状态的用户，设置成offline
 */
void ChatService::Reset() {
    _userModel.ReState();
}

void ChatService::loginOut(const TcpConnectionPtr &conn, json &js, Timestamp timestamp) {
    int userId = js[Agreement::ID].get<int>();
    {
        std::lock_guard<std::mutex> lk(_mtx);
        auto it = _userConnMap.find(userId);
        if (it != _userConnMap.end()) {
            _userConnMap.erase(it);
        }
    }
    //用户注销，相当于就是下线，在redis中取消订阅通道
    _redis.UnSubscribe(userId);
    //更新用户的状态信息
    User user(userId, "", "", Agreement::OFFLINE);
    _userModel.UpdateState(user);
}

void ChatService::handleRedisSubscribeMessage(int userId, std::string msg) {
    std::lock_guard<std::mutex> lk(_mtx);
    auto it = _userConnMap.find(userId);
    if (it != _userConnMap.end()) {
        it->second->Send(msg);
    } else {
        _offlineMsgModel.Insert(userId, msg);
    }
}