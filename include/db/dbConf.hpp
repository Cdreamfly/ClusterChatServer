#pragma once

#include "json.hpp"


class DbConf {
public:
	NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(DbConf, ip_, port_, user_, pwd_, db_,
	                                            minSize_, maxSize_, maxIdleTime_, timeout_)

public:
	std::string ip_ = "127.0.0.1";
	std::string port_ = "3306";
	std::string user_ = "root";
	std::string pwd_ = "cmf.199991";
	std::string db_ = "chat";
	int minSize_{10};
	int maxSize_{64};
	int maxIdleTime_{5000};
	int timeout_{1000};
};
