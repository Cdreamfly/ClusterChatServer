//
// Created by Cmf on 2022/6/2.
//

#ifndef CMFNETLIB_EVENTLOOP_CPP
#define CMFNETLIB_EVENTLOOP_CPP

#include <memory>
#include <functional>
#include <mutex>
#include <atomic>
#include <thread>
#include <vector>
#include "base/Timestamp.hpp"
#include "base/noncopyable.hpp"

class Channel;

class Poller;

class EventLoop : private noncopyable {
public:
    using ptr = std::shared_ptr<EventLoop>;
    using Functor = std::function<void()>;

    EventLoop();

    ~EventLoop();

    void Loop();

    void Quit();

    Timestamp PollReturnTime() const { return _pollReturnTime; }

    /**
     * 在当前loop中执行cb
     * @param cb
     */
    void RunInLoop(Functor cb);

    /**
     * 把cb放入队列中，唤醒loop所在的线程，执行cb
     * @param cb
     */
    void QueueInLoop(Functor cb);

    /**
     * 唤醒所有线程
     */
    void WakeUp();

    void UpdateChannel(Channel *channel);

    void RemoveChannel(Channel *channel);

    bool HasChannel(Channel *channel);

    /**
     * 判断EventLoop对象是否在自己的线程里面
     */
    bool IsInLoopThread() const;


private:
    using ChannelList = std::vector<Channel *>;

    void HandleRead();

    void DoPendingFunctors();

private:
    std::atomic_bool _looping;
    std::atomic_bool _quit; //标识退出loop循环
    const std::thread::id _threadId;  //记录当前loop所在线程的id
    int _wakeupFd;  //linux内核的eventfd创建出来的
    Timestamp _pollReturnTime; //poller返回发生事件的channels的时间点
    std::unique_ptr<Poller> _poller;//eventloop所管理的poller
    std::unique_ptr<Channel> _wakeupChannel;//包括wakeupFd和感兴趣的事件
    ChannelList _activeChannels;    //eventloop所管理的channel
    std::atomic_bool _callingPendingFunctors;   //标识当前loop是否有需要执行的回调操作
    std::vector<Functor> _pendingFunctors;//存储loop需要执行的所有的回调操作
    std::mutex _mtx;//互斥锁，用来保护上面vector容器的线程安全操作
};

#endif //CMFNETLIB_EVENTLOOP_CPP
