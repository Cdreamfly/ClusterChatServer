#pragma once

#include "cm/base/NonCopyable.hpp"

#include <functional>
#include <string>
#include <memory>

namespace cm::net {

	class EventLoop;

	class EventLoopThread;

	class EventLoopThreadPool : private NonCopyable {
	public:
		using ThreadInitCallback = std::function<void(EventLoop *)>;

		explicit EventLoopThreadPool(EventLoop *, std::string);

		~EventLoopThreadPool();

		void setThreadNum(const int numThreads) { numThreads_ = numThreads; }

		void start(const ThreadInitCallback &cb = ThreadInitCallback());

		// 如果工作在多线程中，baseLoop_默认以轮询的方式分配channel给sub loop
		EventLoop *getNextLoop();

		std::vector<EventLoop *> getAllLoops();

		[[nodiscard]] bool started() const { return started_; }

		[[nodiscard]] const std::string &name() const { return name_; }

	private:

		EventLoop *baseLoop_; // EventLoop loop;
		std::string name_;
		bool started_;
		int numThreads_;
		int next_;
		std::vector<std::unique_ptr<EventLoopThread>> threads_;
		std::vector<EventLoop *> loops_;
	};
}