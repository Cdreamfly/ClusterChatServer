//
// Created by Cmf on 2022/5/12.


//

#include "server/model/usermodel.h"
#include <iostream>
#include <sstream>

UserModel::UserModel()
{
    _mySql.Connect();
}

bool UserModel::Insert(User &user)
{
    std::ostringstream sql;
    sql<<"insert into user(name,password,state) values("<<"'"<<user.getName()<<"','"<<user.getPwd()<<"','"<<user.getState()<<"')";
    std::string temp = sql.str();
    std::cout<<"install sql :"<<temp <<std::endl;
    if(_mySql.Update(temp))
    {
        //获取插入成功生成的主键
        user.setId(mysql_insert_id(_mySql.GetConnect()));
        return true;
    }
    return false;
}

User UserModel::Query(int id)
{
    std::ostringstream sql;
    sql<<"select * from user where id = "<<id;
    std::string temp = sql.str();
    std::cout<<"query sql:"<<temp<<std::endl;
    MYSQL_RES* res = _mySql.Query(temp);
    if(res != nullptr)
    {
        MYSQL_ROW row = mysql_fetch_row(res);
        User user(atoi(row[0]),row[1],row[2],row[3]);
        mysql_free_result(res);
        return user;
    }
    return User();
}

bool UserModel::UpdateState(User &user)
{
    std::ostringstream sql;
    sql<<"update user set state = "<<"'"<<user.getState()<<"' where id = "<<user.getId();
    std::string temp = sql.str();
    std::cout<<"updateState sql:"<<temp<<std::endl;
    if(_mySql.Update(temp))
    {
        return true;
    }
    return false;
}

bool UserModel::ReState()
{
    std::string sql = "update user set state = 'offline' where state = 'online'";
    std::cout<<"ReState sql:"<<sql<<std::endl;
    if(_mySql.Update(sql))
    {
        return true;
    }
    return false;
}