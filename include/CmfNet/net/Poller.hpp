//
// Created by Cmf on 2022/6/4.
//

#ifndef CMFNETLIB_POLLER_HPP
#define CMFNETLIB_POLLER_HPP

#include <unordered_map>
#include <vector>
#include "base/noncopyable.hpp"
#include "base/Timestamp.hpp"


/**
 * poller监听的就是eventloop保存的那些channel
 */
class Channel;

class EventLoop;

class Poller : private noncopyable {
public:
    using ChannelList = std::vector<Channel *>;

    Poller(EventLoop *loop);

    virtual ~Poller() = default;

    virtual Timestamp Poll(int timeoutMs, ChannelList *activeChannels) = 0;

    virtual void UpdateChannel(Channel *channel) = 0;

    virtual void RemoveChannel(Channel *channel) = 0;

    /**
     * 判断参数channel是否在当前Poller当中
     * @param channel
     * @return
     */
    bool HasChannel(Channel *channel) const;

    /**
     * EventLoop可以通过该接口获取默认的IO复用的具体实现
     * @param loop
     * @return
     */
    static Poller *NewDefaultPoller(EventLoop *loop);

protected:
    using ChannelMap = std::unordered_map<int, Channel *>;
    ChannelMap _channels;
private:
    EventLoop* _ownerLoop;
};

#endif //CMFNETLIB_POLLER_HPP
