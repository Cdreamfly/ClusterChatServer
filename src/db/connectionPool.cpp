#include "db/connectionPool.hpp"
#include "cm/base/Log.hpp"

using json = nlohmann::json;

ConnectionPool &ConnectionPool::getConnectionPool() {
	static ConnectionPool pool;
	return pool;
}

std::shared_ptr<Connection> ConnectionPool::getConnection() {
	std::unique_lock<std::mutex> lock{mtx_};
	while (connectionQueue_.empty()) {  //连接为空，就阻塞等待connectionTimeout_时间，如果时间过了，还没唤醒
		if (std::cv_status::timeout == cv_.wait_for(lock, std::chrono::microseconds(dbConf_.getTimeout()))) {
			if (connectionQueue_.empty()) { //就可能还是为空
				continue;
			}
		}
	}
	//对于使用完成的连接，不能直接销毁该连接，而是需要将该连接归还给连接池的队列，供之后的其他消费者使用，于是我们使用智能指针，自定义其析构函数，完成放回的操作：
	std::shared_ptr<Connection> res(connectionQueue_.front(), [&](Connection *conn) {
		std::unique_lock<std::mutex> locker{mtx_};
		conn->refreshAliveTime();
		connectionQueue_.push(conn);
	});
	connectionQueue_.pop();
	cv_.notify_all();
	return res;
}

ConnectionPool::ConnectionPool() {
	if (!loadConfigFile()) {
		LOG_ERROR("JSON Config Error");
		return;
	}
	//创建初始数量的连接
	for (int i = 0; i < dbConf_.getMinSize(); ++i) {
		addConnection();
	}
	//启动一个新的线程，作为连接的生产者
	std::thread produce([this] { produceConnectionTask(); });
	produce.detach();//守护线程，主线程结束了，这个线程就结束了
	//启动一个新的定时线程，扫描超过maxIdleTime时间的空闲连接，进行对于的连接回收
	std::thread scanner([this] { scannerConnectionTask(); });
	scanner.detach();
}

ConnectionPool::~ConnectionPool() {
	while (!connectionQueue_.empty()) {
		Connection *ptr = connectionQueue_.front();
		connectionQueue_.pop();
		delete ptr;
	}
}

bool ConnectionPool::loadConfigFile() {
	std::ifstream ifs("../dbConf.json");
	if (!ifs.is_open()) {
		LOG_ERROR("Json config open file error");
		return false;
	}
	json js;
	try {
		js = json::parse(ifs);
	}
	catch (const json::exception &e) {
		LOG_ERROR("JSON Config Error %s", e.what());
		return false;
	}
	dbConf_ = js;
	return true;
}

[[noreturn]] void ConnectionPool::produceConnectionTask() {
	while (true) {
		std::unique_lock<std::mutex> lock(mtx_);
		while (connectionQueue_.size() >= dbConf_.getMinSize()) {
			cv_.wait(lock);
		}
		if (connectionCount_ < dbConf_.getMinSize()) {
			addConnection();
		}
		cv_.notify_all();
	}
}

[[noreturn]] void ConnectionPool::scannerConnectionTask() {
	while (true) {
		std::this_thread::sleep_for(std::chrono::seconds(dbConf_.getMaxIdleTime()));
		std::unique_lock<std::mutex> lock(mtx_);
		while (connectionCount_ > dbConf_.getMinSize()) {
			Connection *ptr = connectionQueue_.front();//队头的时间没超过，那后面的时间就都没超过
			if (ptr->getAliveTime() >= dbConf_.getMaxIdleTime() * 1000) {
				connectionQueue_.pop();
				--connectionCount_;
				delete ptr;
			} else {
				break;
			}
		}
	}
}

void ConnectionPool::addConnection() {
	auto *conn = new Connection();
	if (conn->connect(dbConf_.getIp(), dbConf_.getPort(), dbConf_.getUser(), dbConf_.getPwd(), dbConf_.getDb())) {
		conn->refreshAliveTime();
		connectionQueue_.push(conn);
		++connectionCount_;
	} else {
		LOG_ERROR("MySql Connection Error!");
	}
}