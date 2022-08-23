//
// Created by Cmf on 2022/6/9.
//

#ifndef CMFNETLIB_TCPCONNECTION_HPP
#define CMFNETLIB_TCPCONNECTION_HPP

#include "base/noncopyable.hpp"
#include "InetAddress.hpp"
#include "Callbacks.hpp"
#include "Buffer.hpp"

#include <atomic>

class EventLoop;

class Socket;

class Channel;

class TcpConnection : private noncopyable, public std::enable_shared_from_this<TcpConnection> {
public:
    TcpConnection(EventLoop *loop, const std::string &name, int sockfd,
                  const InetAddress &localAddr,
                  const InetAddress &peerAddr);

    ~TcpConnection();

    void SetConnectionCallback(const ConnectionCallback &cb) { _connectionCallback = cb; }

    void SetMessageCallback(const MessageCallback &cb) { _messageCallback = cb; }

    void SetWriteCompleteCallback(const WriteCompleteCallback &cb) { _writeCompleteCallback = cb; }

    void SetHighWaterMarkCallback(const HighWaterMarkCallback &cb, size_t highWaterMark) {
        _highWaterMarkCallback = cb;
        _highWaterMark = highWaterMark;
    }

    void SetCloseCallback(const ConnectionCallback &cb) { _closeCallback = cb; }

    const std::string &Name() const { return _name; }

    EventLoop *GetLoop() const { return _loop; }

    //发送数据
    void Send(const std::string &buf);

    //关闭连接
    void Shutdown();

    bool Connect() const { return _state == Connected; }

    const InetAddress &LocalAddress() const { return _localAddr; }

    const InetAddress &PeerAddress() const { return _peerAddr; }

    //建立连接
    void ConnectEstablished();

    //连接销毁
    void ConnectDestroyed();

private:
    enum StateE {
        Disconnected,
        Connecting,
        Connected,
        Disconnecting
    };

    void SetState(StateE state) { _state = state; }

    void HandleRead(Timestamp receiveTime);

    void HandleWrite();

    void HandleClose();

    void HandleError();

    void SendInLoop(const std::string&msg);

    void ShutdownInLoop();

private:
    EventLoop *_loop;
    const std::string _name;
    std::atomic_int _state;
    bool _reading;

    std::unique_ptr<Socket> _socket;
    std::unique_ptr<Channel> _channel;

    const InetAddress _localAddr;   //当前主机IP地址端口号
    const InetAddress _peerAddr;//对端IP地址端口号

    ConnectionCallback _connectionCallback;
    MessageCallback _messageCallback;
    WriteCompleteCallback _writeCompleteCallback;
    HighWaterMarkCallback _highWaterMarkCallback;
    CloseCallback _closeCallback;

    size_t _highWaterMark;
    Buffer _inputBuffer;//接收数据的缓冲区
    Buffer _outputBuffer;//发送数据的缓冲区
};


#endif //CMFNETLIB_TCPCONNECTION_HPP
