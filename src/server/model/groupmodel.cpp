//
// Created by Cmf on 2022/5/14.
//

#include "server/model/groupmodel.h"
#include "server/model/groupuser.h"
#include <sstream>

bool GroupModel::createGroup(Group &group) {
    std::ostringstream sql;
    sql << "insert into allgroup(groupname,groupdesc) values('" << group.getName() << "','" << group.getDesc() << "')";
    std::string temp = sql.str();
    std::shared_ptr<Connection> _mySql = ConnectionPool::GetConnectionPool().GetConnection();
    if (_mySql->Update(temp)) {
        group.setId(mysql_insert_id(_mySql->GetConnect()));
        return true;
    }
    return false;
}

void GroupModel::addGroup(const int userId, const int groupId, const std::string &role) {
    std::ostringstream sql;
    sql << "insert into groupuser values(" << groupId << "," << userId << ",'" << role << "')";
    std::string temp = sql.str();
    ConnectionPool::GetConnectionPool().GetConnection()->Update(temp);
}

std::vector<Group> GroupModel::queryGroup(const int userId) {
    std::ostringstream sql;
    sql << "select a.id,a.groupname,a.groupdesc from allgroup a inner join groupuser b on a.id = b.groupid where b.userid = " << userId;
    std::string temp = sql.str();
    MYSQL_RES *res = ConnectionPool::GetConnectionPool().GetConnection()->Query(temp);
    std::vector<Group> groupVec;
    if (res != nullptr) {
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(res)) != nullptr) {
            Group group(atoi(row[0]), row[1], row[2]);
            groupVec.emplace_back(group);
        }
        mysql_free_result(res);
    }
    //查询群组所有群员的信息
    for (auto &it: groupVec) {
        sql.str("");
        sql << "select a.id,a.name,a.state,b.grouprole from user a inner join groupuser b on b.userid = a.id where b.groupid = " << it.getId();
        std::string temp = sql.str();
        MYSQL_RES *res = ConnectionPool::GetConnectionPool().GetConnection()->Query(temp);
        if (res != nullptr) {
            MYSQL_ROW row;
            while ((row = mysql_fetch_row(res)) != nullptr) {
                GroupUser user;
                user.setId(atoi(row[0]));
                user.setName(row[1]);
                user.setState(row[2]);
                user.setRole(row[3]);
                it.getGroupUsers().emplace_back(user);
            }
            mysql_free_result(res);
        }
    }
    return groupVec;
}

std::vector<int> GroupModel::queryGroupUsers(const int userId, const int groupId) {
    std::ostringstream sql;
    sql << "select userid from groupuser where groupid = " << groupId << " and userid != " << userId;
    std::string temp = sql.str();
    MYSQL_RES *res = ConnectionPool::GetConnectionPool().GetConnection()->Query(temp);
    std::vector<int> vec;
    if (res != nullptr) {
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(res)) != nullptr) {
            vec.emplace_back(atoi(row[0]));
        }
        mysql_free_result(res);
    }
    return vec;

}