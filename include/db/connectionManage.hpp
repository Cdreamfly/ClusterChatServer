#pragma once

#include <cm/base/NonCopyable.hpp>
#include "connection.hpp"
#include "dbConf.hpp"

class ConnectionManage {
public:
	static ConnectionManage &getConnectionManage();

	~ConnectionManage();

	bool loadConfigFile();

	ConnectionManage(const ConnectionManage &) = delete;

	ConnectionManage &operator=(const ConnectionManage &) = delete;

	Connection &getConnection();

private:
	ConnectionManage();

private:
	DbConf dbConf_;
	Connection connection_;
};