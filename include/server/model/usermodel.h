//
// Created by Cmf on 2022/5/12.
//

#ifndef CLUSTERCHAT_USERMODEL_H
#define CLUSTERCHAT_USERMODEL_H

#include "user.h"
#include "server/db/ConnectionPool.h"

class UserModel {
public:
    UserModel();

    //添加用户
    bool Insert(User &user);

    //查询用户
    User Query(int id);

    //更新用户状态
    bool UpdateState(User &user);

    //重置用户状态
    bool ReState();

private:
    std::shared_ptr<Connection> _mySql;
};


#endif //CLUSTERCHAT_USERMODEL_H