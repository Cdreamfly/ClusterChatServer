//
// Created by Cmf on 2022/5/31.
//

#ifndef CMFNETLIB_STRINGARG_HPP
#define CMFNETLIB_STRINGARG_HPP

#include <string>

class StringArg {
public:
    StringArg(const char *str) : _str(str) {}

    StringArg(const std::string &str) : _str(str.c_str()) {}

    const char *c_str() {
        return _str;
    }

private:
    const char *_str;
};

#endif //CMFNETLIB_STRINGARG_HPP
