//
// Created by Cmf on 2022/6/9.
//

#ifndef CMFNETLIB_CALLBACKS_HPP
#define CMFNETLIB_CALLBACKS_HPP

#include <functional>
#include <memory>

class Buffer;

class TcpConnection;

class Timestamp;

using TcpConnectionPtr = std::shared_ptr<TcpConnection>;
using ConnectionCallback = std::function<void(const TcpConnectionPtr &)>;
using CloseCallback = std::function<void(const TcpConnectionPtr &)>;
using WriteCompleteCallback = std::function<void(const TcpConnectionPtr &)>;
using MessageCallback = std::function<void(const TcpConnectionPtr &, Buffer *, Timestamp)>;
using HighWaterMarkCallback = std::function<void(const TcpConnectionPtr &, size_t)>;

#endif //CMFNETLIB_CALLBACKS_HPP
