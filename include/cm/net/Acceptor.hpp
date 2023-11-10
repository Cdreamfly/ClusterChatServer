#pragma once

#include "cm/base/NonCopyable.hpp"
#include "cm/net/Channel.hpp"
#include "cm/net/Socket.hpp"

#include <functional>

namespace cm::net {
	class EventLoop;

	class InetAddress;

	class Acceptor : private NonCopyable {
	public:
		using NewConnectionCallback = std::function<void(int, const InetAddress &)>;

		explicit Acceptor(EventLoop *, const InetAddress &, bool);

		~Acceptor();

		void setNewConnectionCallback(const NewConnectionCallback &cb) { newConnectionCallback_ = cb; }

		[[nodiscard]] bool listening() const { return listening_; }

		void listen();

	private:
		EventLoop *loop_; // Acceptor用的就是用户定义的那个baseLoop，也称作mainLoop
		Socket acceptSocket_;
		Channel acceptChannel_;
		NewConnectionCallback newConnectionCallback_;
		bool listening_;
	};
}