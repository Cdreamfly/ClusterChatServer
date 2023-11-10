#pragma once
#include "cm/net/Callbacks.hpp"
#include "cm/base/NonCopyable.hpp"
#include "cm/net/TcpConnection.hpp"
/**
 * 对外服务器编程使用的类
 */
namespace cm::net {
	class Acceptor;

	class EventLoop;

	class EventLoopThreadPool;

	class TcpServer : private NonCopyable {
	public:
		using ThreadInitCallback = std::function<void(EventLoop *)>;

		enum class Option {
			kNoReusePort,
			kReusePort,
		};

		explicit TcpServer(EventLoop *, const InetAddress &, std::string, const Option &option = Option::kNoReusePort);

		~TcpServer();

		void setThreadInitCallback(const ThreadInitCallback &cb) { threadInitCallback_ = cb; }

		void setConnectionCallback(const ConnectionCallback &cb) { connectionCallback_ = cb; }

		void setMessageCallback(const MessageCallback &cb) { messageCallback_ = cb; }

		void setWriteCompleteCallback(const WriteCompleteCallback &cb) { writeCompleteCallback_ = cb; }

		// 设置底层sub loop的个数
		void setThreadNum(int);

		// 开启服务器监听
		void start();

	private:
		void newConnection(int, const InetAddress &);

		using ConnectionMap = std::unordered_map<std::string, TcpConnectionPtr>;

		EventLoop *loop_; // baseLoop 用户定义的loop

		const std::string ipPort_;
		const std::string name_;

		std::unique_ptr<Acceptor> acceptor_; // 运行在mainLoop，任务就是监听新连接事件

		std::shared_ptr<EventLoopThreadPool> threadPool_; // one loop per thread

		ConnectionCallback connectionCallback_; // 有新连接时的回调
		MessageCallback messageCallback_; // 有读写消息时的回调
		WriteCompleteCallback writeCompleteCallback_; // 消息发送完成以后的回调

		ThreadInitCallback threadInitCallback_; // loop线程初始化的回调

		std::atomic_int started_;

		int nextConnId_;
		ConnectionMap connections_; // 保存所有的连接
	};
}