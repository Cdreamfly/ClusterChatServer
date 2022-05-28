//
// Created by Cmf on 2022/5/14.
//

#ifndef CLUSTERCHAT_FRIENDMODEL_H
#define CLUSTERCHAT_FRIENDMODEL_H

#include <vector>
#include "server/model/user.h"
#include "server/db/db.h"

class FriendModel
{
public:
    using ptr = std::shared_ptr<FriendModel>;
    explicit FriendModel();
    //添加好友关系
    void Insert(int userId,int friendId);
    //返回好友列表
    std::vector<User>Query(int userId);
private:
    std::shared_ptr<MySQL>_mySql;
};

#endif //CLUSTERCHAT_FRIENDMODEL_H