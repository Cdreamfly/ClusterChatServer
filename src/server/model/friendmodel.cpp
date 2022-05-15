//
// Created by Cmf on 2022/5/14.
//

#include "server/model/friendmodel.h"
#include <sstream>
FriendModel::FriendModel()
{
    _mySql.Connect();
}
void FriendModel::Insert(int userId, int friendId)
{
    std::ostringstream sql;
    sql<<"insert into friend values("<<userId<<","<<friendId<<")";
    std::string temp = sql.str();
    _mySql.Update(temp);
}

std::vector<User> FriendModel::Query(int userId)
{
    std::ostringstream sql;
    sql<<"select a.id,a.name,a.state from user a inner join friend b on b.friendid = a.id where b.userid = "<<userId;
    std::string temp = sql.str();
    MYSQL_RES*res = _mySql.Query(temp);
    std::vector<User>vec;
    if(res != nullptr)
    {
        MYSQL_ROW row;
        while((row = mysql_fetch_row(res))!= nullptr)
        {
            User user(atoi(row[0]),row[1],row[2],row[3]);
            vec.emplace_back(user);
        }
        mysql_free_result(res);
    }
    return vec;
}