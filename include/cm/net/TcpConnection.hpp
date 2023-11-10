#pragma once

#include "cm/base/NonCopyable.hpp"
#include "cm/net/Callbacks.hpp"
#include "cm/net/InetAddress.hpp"
#include "cm/net/Buffer.hpp"

#include <atomic>

namespace cm::net {
	class Channel;

	class EventLoop;

	class Socket;

	class TcpConnection : private NonCopyable, public std::enable_shared_from_this<TcpConnection> {
	public:
		explicit TcpConnection(EventLoop *, std::string , int, const InetAddress &, const InetAddress &);

		~TcpConnection();

		EventLoop *getLoop() const { return loop_; }

		const std::string &name() const { return name_; }

		const InetAddress &localAddress() const { return localAddr_; }

		const InetAddress &peerAddress() const { return peerAddr_; }

		bool connected() const { return state_ == StateE::kConnected; }

		// 发送数据
		void send(const std::string_view &);

		// 关闭连接
		void shutdown();

		void setConnectionCallback(const ConnectionCallback &cb) { connectionCallback_ = cb; }

		void setMessageCallback(const MessageCallback &cb) { messageCallback_ = cb; }

		void setWriteCompleteCallback(const WriteCompleteCallback &cb) { writeCompleteCallback_ = cb; }

		void setHighWaterMarkCallback(const HighWaterMarkCallback &cb, const size_t highWaterMark) {
			highWaterMarkCallback_ = cb;
			highWaterMark_ = highWaterMark;
		}

		void setCloseCallback(const CloseCallback &cb) { closeCallback_ = cb; }

		// 连接建立
		void connectEstablished();

		// 连接销毁
		void connectDestroyed();

	private:
		enum class StateE {
			kDisconnected, kConnecting, kConnected, kDisconnecting
		};

		void setState(const StateE &state) { state_ = state; }

		void handleRead(Timestamp receiveTime);

		void handleWrite();

		void handleClose();

		void handleError();

		void sendInLoop(const void *, size_t);

		void shutdownInLoop();

		EventLoop *loop_; // 这里绝对不是baseLoop， 因为TcpConnection都是在subLoop里面管理的
		const std::string name_;
		std::atomic<StateE> state_;
		bool reading_;

		// 这里和Acceptor类似   Acceptor=》mainLoop    TcpConenction=》subLoop
		std::unique_ptr<Socket> socket_;
		std::unique_ptr<Channel> channel_;

		const InetAddress localAddr_;
		const InetAddress peerAddr_;

		ConnectionCallback connectionCallback_; // 有新连接时的回调
		MessageCallback messageCallback_; // 有读写消息时的回调
		WriteCompleteCallback writeCompleteCallback_; // 消息发送完成以后的回调
		HighWaterMarkCallback highWaterMarkCallback_;
		CloseCallback closeCallback_;
		size_t highWaterMark_;

		Buffer inputBuffer_;  // 接收数据的缓冲区
		Buffer outputBuffer_; // 发送数据的缓冲区
	};
}