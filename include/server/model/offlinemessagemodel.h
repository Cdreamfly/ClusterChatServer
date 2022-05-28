//
// Created by Cmf on 2022/5/13.
//

#ifndef CLUSTERCHAT_OFFLINEMESSAGEMODEL_H
#define CLUSTERCHAT_OFFLINEMESSAGEMODEL_H

#include <string>
#include <vector>
#include "server/db/db.h"
#include <memory>

//提供离线消息表的操作接口方法
class OfflineMsgModel
{
public:
    using ptr = std::shared_ptr<OfflineMsgModel>;
    explicit OfflineMsgModel();
    //存储离线消息
    void Insert (int id, std::string msg);
    //删除离线消息
    void Remove(int id);
    //查询离线消息
    std::vector<std::string> Query(int id);
private:
    std::shared_ptr<MySQL>_mySql;
};

#endif //CLUSTERCHAT_OFFLINEMESSAGEMODEL_H