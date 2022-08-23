//
// Created by Cmf on 2022/5/31.
//

#ifndef CMFNETLIB_INETADDRESS_HPP
#define CMFNETLIB_INETADDRESS_HPP

#include <arpa/inet.h>
#include <memory>
#include <string.h>
#include "base/StringArg.hpp"
#include "base/copyable.hpp"

class InetAddress : public copyable {
public:
    using ptr = std::shared_ptr<InetAddress>;

    explicit InetAddress() = default;

    explicit InetAddress(uint16_t port,StringArg ip = "127.0.0.1") {
        memset(&_addr, 0, sizeof(_addr));
        _addr.sin_family = AF_INET;
        _addr.sin_addr.s_addr = inet_addr(ip.c_str());
        _addr.sin_port = htons(port);
    }

    InetAddress(StringArg ip, uint16_t port) {
        memset(&_addr, 0, sizeof(_addr));
        _addr.sin_family = AF_INET;
        _addr.sin_port = htons(port);
        inet_pton(AF_INET, ip.c_str(), &_addr.sin_addr);
    }

    explicit InetAddress(const sockaddr_in &addr) : _addr(addr) {

    }

    const sockaddr *GetSockAddr() const {
        return (const sockaddr *) &_addr;
    }

    void SetSockAddr(const sockaddr_in &addr) {
        _addr = addr;
    }

    sa_family_t Family() const {
        return _addr.sin_family;
    }

    std::string GetIP() const {
        char buf[64];
        inet_ntop(AF_INET, &_addr.sin_addr, buf, static_cast<socklen_t>(sizeof(_addr)));
        return buf;
    }

    std::string ToIpPort() const {
        char buf[64] = {0};
        inet_ntop(AF_INET, &_addr.sin_addr, buf, static_cast<socklen_t>(sizeof(_addr)));
        snprintf(buf + strlen(buf), sizeof(buf) - strlen(buf), ":%u", this->GetPort());
        return buf;
    }

    uint16_t GetPort() const {
        return be16toh(_addr.sin_port);
    }


private:
    sockaddr_in _addr;
};

#endif //CMFNETLIB_INETADDRESS_HPP
