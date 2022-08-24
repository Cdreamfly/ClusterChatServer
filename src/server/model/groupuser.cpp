//
// Created by Cmf on 2022/5/14.
//
#include "server/model/groupuser.h"

GroupUser::GroupUser() : _role("") {}

GroupUser::~GroupUser() noexcept {}

void GroupUser::setRole(std::string role) {
    _role = role;
}

std::string GroupUser::getRole() const {
    return _role;
}