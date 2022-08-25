//
// Created by Cmf on 2022/5/12.
//

#include "server/model/user.h"

User::User() : _id(-1), _name(""), _pwd(""), _state("offline") {}

User::User(const int id, const std::string &name, const std::string &pwd, const std::string &state) : _id(id),
                                                                                                      _name(name),
                                                                                                      _pwd(pwd),
                                                                                                      _state(state) {}

void User::setId(const int id) {
    _id = id;
}

void User::setName(const std::string &name) {
    _name = name;
}

void User::setPwd(const std::string &pwd) {
    _pwd = pwd;
}

void User::setState(const std::string &state) {
    _state = state;
}

int User::getId() const {
    return _id;
}

std::string User::getName() const {
    return _name;
}

std::string User::getPwd() const {
    return _pwd;
}

std::string User::getState() const {
    return _state;
}