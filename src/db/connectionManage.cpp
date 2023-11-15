#include <fstream>
#include "db/connectionManage.hpp"
#include "cm/base/Log.hpp"

using json = nlohmann::json;

ConnectionManage &ConnectionManage::getConnectionManage() {
	static ConnectionManage connectionManage;
	return connectionManage;
}

ConnectionManage::~ConnectionManage() = default;

ConnectionManage::ConnectionManage() {
	if (!loadConfigFile()) {
		LOG_ERROR("loading db configure fail!");
		return;
	}
	sql::SQLString url("jdbc:mariadb://" + dbConf_.ip_ + ":" + dbConf_.port_ + "/" + dbConf_.db_);
	sql::Properties properties({{"user",     dbConf_.user_},
	                            {"password", dbConf_.pwd_}});
	connection_.connect(url, properties);
}

bool ConnectionManage::loadConfigFile() {
	std::ifstream ifs("../dbConf.json");
	if (!ifs.is_open()) {
		return false;
	}
	nlohmann::json js;
	ifs >> js;
	dbConf_ = js;
	return true;
}

Connection &ConnectionManage::getConnection() {
	return connection_;
}