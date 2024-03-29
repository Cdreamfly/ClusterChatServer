//
// Created by Cmf on 2022/5/13.
//

#ifndef CLUSTERCHAT_OFFLINEMESSAGEMODEL_H
#define CLUSTERCHAT_OFFLINEMESSAGEMODEL_H

#include <string>
#include <vector>
#include "server/db/ConnectionPool.h"

//提供离线消息表的操作接口方法
class OfflineMsgModel {
public:
    //存储离线消息
    void Insert(const int id, const std::string &msg);

    //删除离线消息
    void Remove(const int id);

    //查询离线消息
    std::vector<std::string> Query(const int id);
};

#endif //CLUSTERCHAT_OFFLINEMESSAGEMODEL_H