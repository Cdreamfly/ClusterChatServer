//
// Created by Cmf on 2022/5/12.
//

#ifndef CLUSTERCHAT_USERMODEL_H
#define CLUSTERCHAT_USERMODEL_H

#include "user.h"
#include "server/db/db.h"

class UserModel
{
public:
    using ptr = std::shared_ptr<UserModel>;
    explicit UserModel();
    //添加用户
    bool Insert(const User::ptr &user);
    //查询用户
    User Query(int id);
    //更新用户状态
    bool UpdateState(const User::ptr &user);
    //重置用户状态
    bool ReState();

private:
    std::shared_ptr<MySQL>_mySql;
};


#endif //CLUSTERCHAT_USERMODEL_H