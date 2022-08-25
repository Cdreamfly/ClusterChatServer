//
// Created by Cmf on 2022/5/14.
//

#include "server/model/group.h"

Group::Group(const int id, const std::string &name, const std::string &desc) : _id(id), _name(name), _desc(desc) {}

void Group::setId(const int id) {
    _id = id;
}

void Group::setName(const std::string &name) {
    _name = name;
}

void Group::setDesc(const std::string &desc) {
    _desc = desc;
}

int Group::getId() const {
    return _id;
}

std::string Group::getName() const {
    return _name;
}

std::string Group::getDesc() const {
    return _desc;
}

std::vector<GroupUser> &Group::getGroupUsers() {
    return _groupUsers;
}