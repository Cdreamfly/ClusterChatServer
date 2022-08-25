//
// Created by Cmf on 2022/5/14.
//

#ifndef CLUSTERCHAT_GROUPMODEL_H
#define CLUSTERCHAT_GROUPMODEL_H

#include "server/db/ConnectionPool.h"
#include "server/model/group.h"

class GroupModel {
public:
    //创建群
    bool createGroup(Group &group);

    //添加群
    void addGroup(const int userId, const int groupId, const std::string&role);

    //查询用户所在群组信息  在客户端呈现
    std::vector<Group> queryGroup(const int userId);

    //查询群里用户id，除userid自己
    std::vector<int> queryGroupUsers(const int userId, const int groupId);
};

#endif //CLUSTERCHAT_GROUPMODEL_H