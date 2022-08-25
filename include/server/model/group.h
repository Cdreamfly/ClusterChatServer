//
// Created by Cmf on 2022/5/14.
//

#ifndef CLUSTERCHAT_GROUP_H
#define CLUSTERCHAT_GROUP_H

#include "server/model/groupuser.h"
#include <vector>

class Group {
public:
    Group(const int id = -1, const std::string &name = "", const std::string &desc = "");

    void setId(const int id);

    void setName(const std::string &name);

    void setDesc(const std::string &desc);

    int getId() const;

    std::string getName() const;

    std::string getDesc() const;

    std::vector<GroupUser> &getGroupUsers();

private:
    int _id;    //群号
    std::string _name;  //群名
    std::string _desc;  //群的功能描述
    std::vector<GroupUser> _groupUsers;  //群成员
};

#endif //CLUSTERCHAT_GROUP_H