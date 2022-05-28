//
// Created by Cmf on 2022/5/14.
//

#ifndef CLUSTERCHAT_GROUPUSER_H
#define CLUSTERCHAT_GROUPUSER_H

#include "server/model/user.h"

/*
 * 群组用户
 */
class GroupUser:public User
{
public:
    using ptr = std::shared_ptr<GroupUser>;
    explicit GroupUser();
    ~GroupUser()noexcept;
    void setRole(std::string role);
    std::string getRole()const;
private:
    std::string _role;  //角色信息（创建者还是成员）
};
#endif //CLUSTERCHAT_GROUPUSER_H