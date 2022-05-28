//
// Created by Cmf on 2022/5/14.
//

#ifndef CLUSTERCHAT_GROUP_H
#define CLUSTERCHAT_GROUP_H

#include "server/model/groupuser.h"
#include <vector>
#include <memory>

class Group
{
public:
    using ptr = std::shared_ptr<Group>;
    explicit Group(int id = -1,std::string name = "",std::string desc = "");

    void setId(int id);
    void setName(std::string name);
    void setDesc(std::string desc);

    int getId()const ;
    std::string getName()const;
    std::string getDesc()const;
    std::vector<GroupUser>& getGroupUsers();

private:
    int _id;    //群号
    std::string _name;  //群名
    std::string _desc;  //群的功能描述
    std::vector<GroupUser>_groupUsers;  //群成员
};

#endif //CLUSTERCHAT_GROUP_H