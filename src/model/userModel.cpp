#include "model/userModel.hpp"
#include "cm/base/Log.hpp"
#include "db/connectionPool.hpp"

bool UserModel::insert(User &user) {
	std::ostringstream sql;
	sql << "insert into user(name,password,state) values(" << "'" << user.getName() << "','" << user.getPwd() << "','"
			<< user.getState() << "')";
	LOG_INFO("install sql :%s", sql.str().c_str());
	std::shared_ptr<Connection> _mySql = ConnectionPool::getConnectionPool().getConnection();
	if (_mySql->update(sql.str())) {
		//获取插入成功生成的主键
		user.setId(static_cast<long>(mysql_insert_id(_mySql->getConnect())));
		return true;
	}
	return false;
}

User UserModel::query(const long id) {
	std::ostringstream sql;
	sql << "select * from user where id = " << id;
	LOG_INFO("query sql: %s", sql.str().c_str());
	MYSQL_RES *res = ConnectionPool::getConnectionPool().getConnection()->query(sql.str());
	if (res != nullptr) {
		if (res->row_count != 0) {
			MYSQL_ROW row = mysql_fetch_row(res);
			User user(std::stol(row[0]), row[1], row[2], row[3]);
			mysql_free_result(res);
			return user;
		}
	}
	return {};
}

bool UserModel::updateState(const User &user) {
	std::ostringstream sql;
	sql << "update user set state = " << "'" << user.getState() << "' where id = " << user.getId();
	LOG_INFO("update State sql: %s", sql.str().c_str());
	if (ConnectionPool::getConnectionPool().getConnection()->update(sql.str())) {
		return true;
	}
	return false;
}

bool UserModel::reState() {
	const std::string_view sql = "update user set state = 'offline' where state = 'online'";
	LOG_INFO("ReState sql: %s", sql);
	if (ConnectionPool::getConnectionPool().getConnection()->update(sql)) {
		return true;
	}
	return false;
}
