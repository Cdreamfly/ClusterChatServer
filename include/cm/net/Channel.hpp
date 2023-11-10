#pragma once

#include <functional>
#include <memory>

#include "cm/base/Timestamp.hpp"
#include "cm/base/NonCopyable.hpp"

/**
 * Channel理解为通道，封装了sockFd和其感兴趣的event，如EPOLLIN,EPOLLOUT事件，还绑定了Poller返回的具体事件
 */

namespace cm::net {
	class EventLoop;

	class Channel : private NonCopyable {
	public:
		using EventCallback = std::function<void()>;
		using ReadEventCallback = std::function<void(Timestamp)>;

		explicit Channel(EventLoop *, int);

		~Channel();

		void handleEvent(Timestamp);

		void setReadCallback(ReadEventCallback cb) { readCallback_ = std::move(cb); }

		void setWriteCallback(EventCallback cb) { writeCallback_ = std::move(cb); }

		void setCloseCallback(EventCallback cb) { closeCallback_ = std::move(cb); }

		void setErrorCallback(EventCallback cb) { errorCallback_ = std::move(cb); }

		void tie(const std::shared_ptr<void> &);

		[[nodiscard]] int fd() const { return fd_; }

		[[nodiscard]] int events() const { return events_; }

		void setReceivedEvents_(const int event) { receivedEvents_ = event; } // used by pollers

		[[nodiscard]] bool isNoneEvent() const { return events_ == kNoneEvent; }

		void enableReading() {
			events_ |= kReadEvent;
			update();
		}

		void disableReading() {
			events_ &= ~kReadEvent;
			update();
		}

		void enableWriting() {
			events_ |= kWriteEvent;
			update();
		}

		void disableWriting() {
			events_ &= ~kWriteEvent;
			update();
		}

		void disableAll() {
			events_ = kNoneEvent;
			update();
		}

		[[nodiscard]] bool isWriting() const { return events_ & kWriteEvent; }

		[[nodiscard]] bool isReading() const { return events_ & kReadEvent; }

		[[nodiscard]] int index() const { return index_; }

		void setIndex(const int idx) { index_ = idx; }

		EventLoop *ownerLoop() { return loop_; }

		void remove();

	private:

		void update();

		void handleEventWithGuard(Timestamp);

		static const int kNoneEvent;
		static const int kReadEvent;
		static const int kWriteEvent;

		EventLoop *loop_;
		const int fd_;
		int events_;
		int receivedEvents_; // it's the received event types of epoll or poll
		int index_; // used by Poller.

		std::weak_ptr<void> tie_;
		bool tied_;

		ReadEventCallback readCallback_;
		EventCallback writeCallback_;
		EventCallback closeCallback_;
		EventCallback errorCallback_;
	};
}