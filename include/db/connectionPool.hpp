#pragma once

#include <cm/base/NonCopyable.hpp>
#include <memory>
#include <queue>
#include <mutex>
#include <atomic>
#include <condition_variable>

#include "db/connection.hpp"
#include "db/dbConf.hpp"

class ConnectionPool : private cm::NonCopyable {
public:
	static ConnectionPool &getConnectionPool(); //获取连接池对象实例
	//给外部提供接口，从连接池中获取一个可用的空闲连接
	std::shared_ptr<Connection> getConnection();//智能指针自动管理连接的释放

	~ConnectionPool();

private:
	ConnectionPool();

	bool loadConfigFile();

	//运行在独立的线程中，专门负责生产新连接
	[[noreturn]] void produceConnectionTask();

	//扫描超过maxIdleTime时间的空闲连接，进行对于的连接回收
	[[noreturn]] void scannerConnectionTask();

	void addConnection();

private:
	DbConf dbConf_;
	std::queue<Connection *> connectionQueue_;//存储连接队列
	std::mutex mtx_;    //维护连接队列的线程安全互斥锁
	std::atomic_int connectionCount_{};//记录连接所创建的connection连接的总数量
	std::condition_variable cv_;//设置条件变量，用于连接生产线程和连接消费线程的通信
};