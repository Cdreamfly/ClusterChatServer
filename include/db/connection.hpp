#pragma once

#include <chrono>
#include <string_view>
#include <mysql/mysql.h>

class Connection {
public:
	Connection();

	~Connection();

	bool connect(const std::string_view &, uint16_t, const std::string_view &,
	             const std::string_view &, const std::string_view &);

	bool update(const std::string_view &);

	MYSQL_RES *query(const std::string_view &);

	//获取链接句柄
	[[nodiscard]] MYSQL *getConnect() const;

	void refreshAliveTime();

	[[nodiscard]] long long getAliveTime() const;

private:
	MYSQL *conn_{};
	std::chrono::time_point<std::chrono::steady_clock> aliveTime_;
};