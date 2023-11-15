#pragma once

#include <mariadb/conncpp.hpp>
#include <iostream>
#include <chrono>

class Connection {
public:
	Connection();

	~Connection();

	bool connect(const sql::SQLString &, sql::Properties &);

	sql::ResultSet *showTasks(const std::string_view &);

	void updateTask(const std::string_view &);

private:
	std::unique_ptr<sql::Connection> conn_;
};