#pragma once

#include "model/user.hpp"

class UserModel {
public:
	//添加用户
	static bool insert(User&);

	//查询用户
	static User query(long);

	//更新用户状态
	static bool updateState(const User &);

	//重置用户状态
	static bool reState();
};