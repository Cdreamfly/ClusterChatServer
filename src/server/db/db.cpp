//
// Created by Cmf on 2022/5/12.
//

#include "server/db/db.h"
#include "CmfNet/base/Log.hpp"

static const std::string HOST = "127.0.0.1";
static const unsigned int PORT = 3306;
static const std::string USER = "root";
static const std::string PWD = "cmf.199991";
static const std::string DB = "chat";

MySQL::MySQL()
{
    conn_ = mysql_init(nullptr);
}

MySQL::~MySQL()
{
    if(conn_!= nullptr)
    {
        mysql_close(conn_);
    }
}

MYSQL *MySQL::GetConnect()
{
    return conn_;
}

bool MySQL::Connect()
{
    conn_ = mysql_real_connect(conn_,HOST.c_str(),USER.c_str(),PWD.c_str(),DB.c_str(),PORT, nullptr,0);
    if(conn_ == nullptr)
    {
        LOG_ERROR("MySQL Connect ERROR!");
        return false;
    }
    return true;
}

MYSQL_RES *MySQL::Query(std::string& sql)
{
    if(mysql_query(conn_,sql.c_str()) != 0)
    {
        LOG_INFO("SQL %s 查询失败：%d",sql.c_str(),mysql_error(conn_));
        return nullptr;
    }
    return mysql_use_result(conn_);
}

bool MySQL::Update(std::string &sql)
{
    if(mysql_query(conn_,sql.c_str()) != 0)
    {
        LOG_INFO("SQL %s 更新失败：%d",sql.c_str(),mysql_error(conn_));
        return false;
    }
    return true;
}