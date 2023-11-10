#pragma once

#include "cm/base/NonCopyable.hpp"
#include "cm/base/Thread.hpp"

#include <condition_variable>

namespace cm::net {
	class EventLoop;

	class EventLoopThread : private NonCopyable {
	public:
		using ThreadInitCallback = std::function<void(EventLoop*)>;

		explicit EventLoopThread(ThreadInitCallback cb = ThreadInitCallback(),
		                const std::string &name = std::string());
		~EventLoopThread();

		EventLoop* startLoop();
	private:
		void threadFunc();

		EventLoop *loop_;
		bool exiting_;
		Thread thread_;
		std::mutex mutex_;
		std::condition_variable cond_;
		ThreadInitCallback callback_;
	};
}