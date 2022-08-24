//
// Created by Cmf on 2022/8/24.
//

#ifndef CLUSTERCHATSERVER_COMMOMCONNECTIONPOOL_H
#define CLUSTERCHATSERVER_COMMOMCONNECTIONPOOL_H

#include <CmfNet/base/noncopyable.hpp>
#include <memory>
#include <queue>
#include <mutex>
#include <atomic>
#include <thread>
#include <condition_variable>

#include "server/db/Connection.h"

class ConnectionPool : private noncopyable {
public:
    static ConnectionPool &GetConnectionPool(); //获取连接池对象实例
    //给外部提供接口，从连接池中获取一个可用的空闲连接
    std::shared_ptr<Connection> GetConnection();//智能指针自动管理连接的释放

    ~ConnectionPool();

private:
    ConnectionPool();

    bool LoadConfigFile();

    //运行在独立的线程中，专门负责生产新连接
    void ProduceConnectionTask();

    //扫描超过maxIdleTime时间的空闲连接，进行对于的连接回收
    void ScannerConnectionTask();

    void AddConnection();

private:
    std::string _ip;
    uint16_t _port;
    std::string _user;
    std::string _pwd;
    std::string _db;

    size_t _minSize;  //初始链接数量
    size_t _maxSize;   //最大连接数量
    size_t _maxIdleTime;//最大空闲时间
    size_t _connectionTimeout;//超时时间

    std::queue<Connection *> _connectionQueue;//存储连接队列
    std::mutex _mtx;    //维护连接队列的线程安全互斥锁
    std::atomic_int _connectionCount;//记录连接所创建的connection连接的总数量
    std::condition_variable _cv;//设置条件变量，用于连接生产线程和连接消费线程的通信
};

#endif //CLUSTERCHATSERVER_COMMOMCONNECTIONPOOL_H
