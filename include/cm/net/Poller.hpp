#pragma once

#include "cm/base/NonCopyable.hpp"
#include "cm/base/Timestamp.hpp"

#include <vector>
#include <unordered_map>

namespace cm::net {
	class Channel;

	class EventLoop;

	class Poller : private NonCopyable {
	public:
		using ChannelList = std::vector<Channel *>;

		explicit Poller(EventLoop *);

		virtual ~Poller() = default;

		// 给所有IO复用保留统一的接口
		virtual Timestamp poll(int, ChannelList *) = 0;

		virtual void updateChannel(Channel *) = 0;

		virtual void removeChannel(Channel *) = 0;

		// 判断参数channel是否在当前Poller当中
		bool hasChannel(Channel *) const;

		// EventLoop可以通过该接口获取默认的IO复用的具体实现
		static Poller *newDefaultPoller(EventLoop *);

	protected:
		// map的key：sock fd  value：sock fd所属的channel通道类型
		using ChannelMap = std::unordered_map<int, Channel *>;
		ChannelMap channels_;
	private:
		EventLoop *ownerLoop_; // 定义Poller所属的事件循环EventLoop
	};
}