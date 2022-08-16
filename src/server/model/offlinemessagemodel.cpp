//
// Created by Cmf on 2022/5/13.
//

#include "server/model/offlinemessagemodel.h"
#include <sstream>

OfflineMsgModel::OfflineMsgModel()
{
    _mySql.Connect();
}

void OfflineMsgModel::Insert(int id, std::string msg)
{
    std::ostringstream sql;
    sql<<"insert into offlinemessage values('"<<id<<"','"<<msg<<"')";
    std::string temp = sql.str();
    _mySql.Update(temp);
}

void OfflineMsgModel::Remove(int id)
{
    std::ostringstream sql;
    sql<<"delete from offlinemessage where userid="<<id;
    std::string temp = sql.str();
    _mySql.Update(temp);
}

std::vector<std::string> OfflineMsgModel::Query(int id)
{
    std::ostringstream sql;
    sql<<"select message from offlinemessage where userid ="<<id;
    std::string temp = sql.str();
    MYSQL_RES*res = _mySql.Query(temp);
    std::vector<std::string>vec;
    if(res != nullptr)
    {
        MYSQL_ROW row;
        while((row = mysql_fetch_row(res)) != nullptr)
        {
            vec.emplace_back(row[0]);//加入数组中
        }
        mysql_free_result(res);
    }
    return vec;
}