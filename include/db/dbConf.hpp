#pragma once

#include "json.hpp"


class DbConf {
public:
	NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(DbConf, ip_, port_, user_, pwd_, db_,
	                                            minSize_, maxSize_, maxIdleTime_, timeout_)

	[[nodiscard]] const std::string &getIp() const {
		return ip_;
	}

	[[nodiscard]] int getPort() const {
		return port_;
	}

	[[nodiscard]] const std::string &getUser() const {
		return user_;
	}

	[[nodiscard]] const std::string &getPwd() const {
		return pwd_;
	}

	[[nodiscard]] const std::string &getDb() const {
		return db_;
	}

	[[nodiscard]] int getMinSize() const {
		return minSize_;
	}

	[[nodiscard]] int getMaxSize() const {
		return maxSize_;
	}

	[[nodiscard]] int getMaxIdleTime() const {
		return maxIdleTime_;
	}

	[[nodiscard]] int getTimeout() const {
		return timeout_;
	}

private:
	std::string ip_ = "127.0.0.1";
	int port_ = 3306;
	std::string user_ = "root";
	std::string pwd_ = "cmf.199991";
	std::string db_ = "chat";
	int minSize_{10};
	int maxSize_{64};
	int maxIdleTime_{5000};
	int timeout_{1000};
};
