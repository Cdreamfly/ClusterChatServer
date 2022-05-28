//
// Created by Cmf on 2022/5/12.
//

#ifndef CLUSTERCHAT_DB_H
#define CLUSTERCHAT_DB_H

#include <mysql/mysql.h>
#include <string>

class MySQL
{
public:
    //初始化
    explicit MySQL();
    //释放
    ~MySQL()noexcept;
    //链接数据库
    bool Connect();
    //更新操作
    bool Update(std::string&);
    //查新操作
    MYSQL_RES*Query(std::string&);
    //获取链接句柄
    MYSQL* GetConnect();

private:
    MYSQL *_conn; //句柄
};
#endif //CLUSTERCHAT_DB_H