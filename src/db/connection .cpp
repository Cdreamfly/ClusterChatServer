#include "db/connection.hpp"

Connection::Connection() = default;

Connection::~Connection() { conn_->close(); }

sql::ResultSet *Connection::showTasks(const std::string_view &sql) {
	sql::ResultSet *res = nullptr;
	try {
		res = conn_->createStatement()->executeQuery(sql.data());
		while (res->next()) {
			std::cout << "id = " << res->getInt(1);
			std::cout << ", description = " << res->getString(2);
			std::cout << ", completed = " << res->getBoolean(3) << "\n";
		}
	}
	catch (sql::SQLException &e) {
		std::cerr << "Error selecting tasks: " << e.what() << std::endl;
	}
	return res;
}

void Connection::updateTask(const std::string_view &sql) {
	try {
		conn_->prepareStatement(sql.data())->executeQuery();
	}
	catch (sql::SQLException &e) {
		std::cerr << "Error updating task: " << e.what() << std::endl;
	}
}

bool Connection::connect(const sql::SQLString &url, sql::Properties &properties) {
	conn_.reset(sql::DriverManager::getConnection(url, properties));
	return true;
}