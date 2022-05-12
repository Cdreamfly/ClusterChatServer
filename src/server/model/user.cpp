//
// Created by Cmf on 2022/5/12.
//

#include "server/model/user.h"

User::User():id_(-1),name_(""),pwd_(""),state_("offline") {}

User::User(int id, std::string name, std::string pwd, std::string state):id_(id),name_(name),pwd_(pwd),state_(state){}

User::~User() {}

void User::setId(int id)
{
    id_ = id;
}

void User::setName(std::string name)
{
    name_ = name;
}

void User::setPwd(std::string pwd)
{
    pwd_ = pwd;
}

void User::setState(std::string state)
{
    state_ = state;
}

int User::getId() const
{
    return id_;
}

std::string User::getName() const
{
    return name_;
}

std::string User::getPwd() const
{
    return pwd_;
}

std::string User::getState() const
{
    return state_;
}
