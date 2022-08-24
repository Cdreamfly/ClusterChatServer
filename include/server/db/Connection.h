//
// Created by Cmf on 2022/8/24.
//

#ifndef CLUSTERCHATSERVER_CONNECTION_H
#define CLUSTERCHATSERVER_CONNECTION_H

#include <mysql/mysql.h>
#include <chrono>
#include <string>

#include "CmfNet/base/Log.hpp"

class Connection {
public:
    Connection();

    ~Connection();

    bool Connect(const std::string &ip, const uint16_t port, const std::string &user, const std::string &pwd,
                 const std::string &db);

    bool Update(const std::string &sql);

    MYSQL_RES *Query(const std::string &sql);

    //获取链接句柄
    MYSQL *GetConnect() const;

    void RefreshAliveTime();

    long long GetAliveTime() const;

private:
    MYSQL *_conn;
    std::chrono::time_point<std::chrono::steady_clock> _aliveTime;
};

#endif //CLUSTERCHATSERVER_CONNECTION_H
