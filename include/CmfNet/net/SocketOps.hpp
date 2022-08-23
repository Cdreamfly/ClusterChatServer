//
// Created by Cmf on 2022/5/31.
//

#pragma once

#include "base/Log.hpp"
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/uio.h>


namespace SocketOps {
    int CreateNonblockingSocket(sa_family_t family);

    void Bind(int fd, const sockaddr *addr);

    void Listen(int fd);

    int Accept(int fd, sockaddr_in *addr);

    void Close(int fd);

    void ShutdownWrite(int fd);

    ssize_t Readv(int fd, const iovec *iov, int iovcnt);

    ssize_t Write(int fd, const void *buf, size_t size);
}