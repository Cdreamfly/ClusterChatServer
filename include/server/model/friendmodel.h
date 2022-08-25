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
    //添加好友关系
    void Insert(const int userId, const int friendId);

    //返回好友列表
    std::vector<User> Query(const int userId);
};

#endif //CLUSTERCHAT_FRIENDMODEL_H