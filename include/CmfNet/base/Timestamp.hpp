//
// Created by Cmf on 2022/6/2.
//

#ifndef CMFNETLIB_TIMESTAMP_HPP
#define CMFNETLIB_TIMESTAMP_HPP

#include <stdint.h>
#include <chrono>
#include <string>
#include <memory>
#include "copyable.hpp"

class Timestamp : public copyable {
public:
    using ptr = std::shared_ptr<Timestamp>;

    explicit Timestamp() : _microSecondsSinceEpoch(0) {}

    explicit Timestamp(int64_t microSecondsSinceEpoch) : _microSecondsSinceEpoch(microSecondsSinceEpoch) {}

    static Timestamp Now() {
        return Timestamp(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));
    }

    std::string ToString() const {
        tm *ptm = localtime(&_microSecondsSinceEpoch);
        char date[128] = {0};
        sprintf(date, "%4d-%02d-%02d %02d:%02d:%02d",
                static_cast<int>(ptm->tm_year + 1900),
                static_cast<int>(ptm->tm_mon + 1),
                static_cast<int>(ptm->tm_mday),
                static_cast<int>(ptm->tm_hour),
                static_cast<int>(ptm->tm_min),
                static_cast<int>(ptm->tm_sec));
        return std::move(std::string(date));
    }

private:
    int64_t _microSecondsSinceEpoch;
};


#endif //CMFNETLIB_TIMESTAMP_HPP
