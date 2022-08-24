//
// Created by Cmf on 2022/5/12.
//

#ifndef CLUSTERCHAT_USER_H
#define CLUSTERCHAT_USER_H

#include<string>

class User {
public:
    User();

    User(int id, std::string name, std::string pwd, std::string state = "offline");

    virtual ~User() {}

    void setId(int id);

    void setName(std::string name);

    void setPwd(std::string pwd);

    void setState(std::string state);

    int getId() const;

    std::string getName() const;

    std::string getPwd() const;

    std::string getState() const;

private:
    int _id;
    std::string _name;
    std::string _pwd;
    std::string _state;
};

#endif //CLUSTERCHAT_USER_H