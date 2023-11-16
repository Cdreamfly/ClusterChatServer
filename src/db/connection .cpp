#include "db/connection.hpp"
#include "cm/base/Log.hpp"

Connection::Connection() {
	conn_ = mysql_init(nullptr);
	mysql_set_character_set(conn_, "utf8");
}

Connection::~Connection() {
	if (conn_ != nullptr) {
		mysql_close(conn_);
	}
}

long long Connection::getAliveTime() const {
	return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - aliveTime_).count();
}

bool Connection::connect(const std::string_view &ip, const uint16_t port,
                         const std::string_view &user, const std::string_view &pwd,
                         const std::string_view &db) {
	conn_ = mysql_real_connect(conn_, ip.data(), user.data(), pwd.data(), db.data(), port, nullptr, 0);
	if (conn_ == nullptr) {
		LOG_ERROR("MySQL Connect Error!")
		return false;
	}
	return true;
}

bool Connection::update(const std::string_view &sql) {
	if (mysql_query(conn_, sql.data()) != 0) {
		LOG_INFO("SQL %s update fail：%d", sql.data(), mysql_error(conn_));
		return false;
	}
	return true;

}

MYSQL_RES *Connection::query(const std::string_view &sql) {
	if (mysql_query(conn_, sql.data()) != 0) {
		LOG_INFO("SQL %s query fail：%d", sql.data(), mysql_error(conn_));
		return nullptr;
	}
	return mysql_use_result(conn_);
}

MYSQL *Connection::getConnect() const {
	return conn_;
}

void Connection::refreshAliveTime() {
	aliveTime_ = std::chrono::steady_clock::now();
}
