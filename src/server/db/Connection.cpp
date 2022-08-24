//
// Created by Cmf on 2022/8/24.
//

#include "server/db/Connection.h"

Connection::Connection() {
    _conn = mysql_init(nullptr);
    mysql_set_character_set(_conn, "utf8");//设置编码格式维utf8
}

Connection::~Connection() {
    if (_conn != nullptr) {
        mysql_close(_conn);
    }
}

bool Connection::Connect(const std::string &ip, const uint16_t port, const std::string &user, const std::string &pwd,
                         const std::string &db) {
    _conn = mysql_real_connect(_conn, ip.c_str(), user.c_str(), pwd.c_str(), db.c_str(), port, nullptr, 0);
    if (_conn == nullptr) {
        LOG_ERROR("MySQL Connect Error")
        return false;
    }
    return true;
}

bool Connection::Update(const std::string &sql) {
    if (mysql_query(_conn, sql.c_str()) != 0) {
        LOG_INFO("SQL %s 更新失败：%d", sql.c_str(), mysql_error(_conn));
        return false;
    }
    return true;
}

MYSQL_RES *Connection::Query(const std::string &sql) {
    if (mysql_query(_conn, sql.c_str()) != 0) {
        LOG_INFO("SQL %s 查询失败：%d", sql.c_str(), mysql_error(_conn));
        return nullptr;
    }
    return mysql_use_result(_conn);
}

MYSQL *Connection::GetConnect() const {
    return _conn;
}

void Connection::RefreshAliveTime() {
    _aliveTime = std::chrono::steady_clock::now();
}

long long Connection::GetAliveTime() const {
    return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - _aliveTime).count();
}
