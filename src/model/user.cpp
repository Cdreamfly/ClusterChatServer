#include "model/user.hpp"

User::User(const int id, std::string_view &name, std::string_view &pwd, std::string_view state) : id_(id),
                                                                                                  name_(name),
                                                                                                  pwd_(pwd),
                                                                                                  state_(state) {}

void User::setName(const std::string_view &name) {

}

void User::setId(const int id) {

}

void User::setPwd(const std::string_view &pwd) {

}

void User::setState(const std::string_view &state) {

}

int User::getId() const {
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
