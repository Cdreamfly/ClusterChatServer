//
// Created by Cmf on 2022/5/14.
//

#ifndef CLUSTERCHAT_FRIENDMODEL_H
#define CLUSTERCHAT_FRIENDMODEL_H

#include <vector>
#include "server/model/user.h"
#include "server/db/ConnectionPool.h"

class FriendModel {
public:
    FriendModel();

    //添加好友关系
    void Insert(int userId, int friendId);

    //返回好友列表
    std::vector<User> Query(int userId);

private:
    std::shared_ptr<Connection> _mySql;
};

#endif //CLUSTERCHAT_FRIENDMODEL_H