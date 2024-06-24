#include "model/user.hpp"

User::User(const long id, const std::string_view &name, const std::string_view &pwd, const std::string_view state)
		: id_(id),
		  name_(name),
		  pwd_(pwd),
		  state_(state) {}

void User::setName(const std::string_view &name) {
	name_ = name;
}

void User::setId(const long id) {
	id_ = id;
}

void User::setPwd(const std::string_view &pwd) {
	pwd_ = pwd;
}

void User::setState(const std::string_view &state) {
	state_ = state;
}

long User::getId() const {
	return id_;
}

std::string_view User::getName() const {
	return name_;
}

std::string_view User::getPwd() const {
	return pwd_;
}

std::string_view User::getState() const {
	return state_;
}
