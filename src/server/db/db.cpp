//
// Created by Cmf on 2022/5/12.
//

#include "server/db/db.h"
#include "muduo/base/Logging.h"

static const std::string HOST = "127.0.0.1";
static const unsigned int PORT = 3306;
static const std::string USER = "root";
static const std::string PWD = "cmf.199991";
static const std::string DB = "chat";

MySQL::MySQL()
{
    _conn = mysql_init(nullptr);
}

MySQL::~MySQL()
{
    if(_conn!= nullptr)
    {
        mysql_close(_conn);
    }
}

MYSQL *MySQL::GetConnect()
{
    return _conn;
}

bool MySQL::Connect()
{
    _conn = mysql_real_connect(_conn,HOST.c_str(),USER.c_str(),PWD.c_str(),DB.c_str(),PORT, nullptr,0);
    if(_conn == nullptr)
    {
        LOG_ERROR<<"MySQL Connect ERROR!";
        return false;
    }
    return true;
}

MYSQL_RES *MySQL::Query(std::string& sql)
{
    if(mysql_query(_conn,sql.c_str()) != 0)
    {
        LOG_INFO<<__FILE__<<":"<<__LINE__<<":"<<sql<<"查询失败:"<< mysql_error(_conn);
        return nullptr;
    }
    return mysql_use_result(_conn);
}

bool MySQL::Update(std::string &sql)
{
    if(mysql_query(_conn,sql.c_str()) != 0)
    {
        LOG_INFO<<__FILE__<<":"<<__LINE__<<":"<<sql<<"更新失败:"<< mysql_error(_conn);
        return false;
    }
    return true;
}