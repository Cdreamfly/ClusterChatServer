//
// Created by Cmf on 2022/8/24.
//
#include <fstream>
#include "server/db/ConnectionPool.h"
#include "json.hpp"

using json = nlohmann::json;

ConnectionPool &ConnectionPool::GetConnectionPool() {
    static ConnectionPool pool;
    return pool;
}

std::shared_ptr<Connection> ConnectionPool::GetConnection() {
    std::unique_lock<std::mutex> lock(_mtx);
    while (_connectionQueue.empty()) {  //连接为空，就阻塞等待_connectionTimeout时间，如果时间过了，还没唤醒
        if (std::cv_status::timeout == _cv.wait_for(lock, std::chrono::microseconds(_connectionTimeout))) {
            if (_connectionQueue.empty()) { //就可能还是为空
                continue;
            }
        }
    }
    //对于使用完成的连接，不能直接销毁该连接，而是需要将该连接归还给连接池的队列，供之后的其他消费者使用，于是我们使用智能指针，自定义其析构函数，完成放回的操作：
    std::shared_ptr<Connection> res(_connectionQueue.front(), [&](Connection *conn) {
        std::unique_lock<std::mutex> locker(_mtx);
        conn->RefreshAliveTime();
        _connectionQueue.push(conn);
    });
    _connectionQueue.pop();
    _cv.notify_all();
    return res;
}

ConnectionPool::ConnectionPool() {
    if (!LoadConfigFile()) {
        LOG_ERROR("JSON Config Error");
        return;
    }
    //创建初始数量的连接
    for (int i = 0; i < _minSize; ++i) {
        AddConnection();
    }
    //启动一个新的线程，作为连接的生产者 linux thread => pthread_create
    std::thread produce(std::bind(&ConnectionPool::ProduceConnectionTask, this));
    produce.detach();//守护线程，主线程结束了，这个线程就结束了
    //启动一个新的定时线程，扫描超过maxIdleTime时间的空闲连接，进行对于的连接回收
    std::thread scanner(std::bind(&ConnectionPool::ScannerConnectionTask, this));
    scanner.detach();
}

ConnectionPool::~ConnectionPool() {
    while (!_connectionQueue.empty()) {
        Connection *ptr = _connectionQueue.front();
        _connectionQueue.pop();
        delete ptr;
    }
}

bool ConnectionPool::LoadConfigFile() {
    std::ifstream ifs("../../dbconf.json");
    json js;
    ifs >> js;
    if (!js.is_object()) {
        LOG_ERROR("JSON is NOT Object");
        return false;
    }
    if (!js["ip"].is_string() ||
        !js["port"].is_number() ||
        !js["user"].is_string() ||
        !js["pwd"].is_string() ||
        !js["db"].is_string() ||
        !js["minSize"].is_number() ||
        !js["maxSize"].is_number() ||
        !js["maxIdleTime"].is_number() ||
        !js["timeout"].is_number()) {
        LOG_ERROR("JSON The data type does not match");
        return false;
    }
    _ip = js["ip"].get<std::string>();
    _port = js["port"].get<uint16_t>();
    _user = js["user"].get<std::string>();
    _pwd = js["pwd"].get<std::string>();
    _db = js["db"].get<std::string>();
    _minSize = js["minSize"].get<size_t>();
    _maxSize = js["maxSize"].get<size_t>();
    _maxIdleTime = js["maxIdleTime"].get<size_t>();
    _connectionTimeout = js["timeout"].get<size_t>();
    return true;
}

void ConnectionPool::ProduceConnectionTask() {
    while (true) {
        std::unique_lock<std::mutex> lock(_mtx);
        while (_connectionQueue.size() >= _minSize) {
            _cv.wait(lock);
        }
        if (_connectionCount < _maxSize) {
            AddConnection();
        }
        _cv.notify_all();
    }
}

void ConnectionPool::ScannerConnectionTask() {
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(_maxIdleTime));
        std::unique_lock<std::mutex> lock(_mtx);
        while (_connectionCount > _minSize) {
            Connection *ptr = _connectionQueue.front();//队头的时间没超过，那后面的时间就都没超过
            if (ptr->GetAliveTime() >= _maxIdleTime * 1000) {
                _connectionQueue.pop();
                --_connectionCount;
                delete ptr;
            } else {
                break;
            }
        }
    }
}

void ConnectionPool::AddConnection() {
    Connection *conn = new Connection();
    conn->Connect(_ip, _port, _user, _pwd, _db);
    conn->RefreshAliveTime();
    _connectionQueue.push(conn);
    ++_connectionCount;
}