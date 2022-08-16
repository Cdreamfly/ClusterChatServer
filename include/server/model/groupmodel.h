//
// Created by Cmf on 2022/5/14.
//

#ifndef CLUSTERCHAT_GROUPMODEL_H
#define CLUSTERCHAT_GROUPMODEL_H

#include "server/db/db.h"
#include "server/model/group.h"

class GroupModel
{
public:
    GroupModel();
    ~GroupModel();
    //创建群
    bool createGroup(Group&group);
    //添加群
    void addGroup(int userId,int groupId,std::string role);
    //查询用户所在群组信息  在客户端呈现
    std::vector<Group> queryGroup(int userId);
    //查询群里用户id，除userid自己
    std::vector<int>queryGroupUsers(int userId,int groupId);
private:
    MySQL _mySql;
};

#endif //CLUSTERCHAT_GROUPMODEL_H