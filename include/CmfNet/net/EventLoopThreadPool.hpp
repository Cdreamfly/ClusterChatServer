//
// Created by Cmf on 2022/6/8.
//

#ifndef CMFNETLIB_EVENTLOOPTHREADPOOL_HPP
#define CMFNETLIB_EVENTLOOPTHREADPOOL_HPP

#include "base/noncopyable.hpp"
#include "base/Thread.hpp"
#include <functional>
#include <vector>

class EventLoop;

class EventLoopThread;

class EventLoopThreadPool : private noncopyable {
public:
    using ThreadInitCallback = std::function<void(EventLoop *)>;

    EventLoopThreadPool(EventLoop *baseLoop, const std::string &nameArg);

    ~EventLoopThreadPool();

    /**
     * 设置底层线程的数量，TcpServer::setThreadNum底层调用的就是EventLoopThreadPool::setThreadNum
     * @param numThreads
     */
    void SetThreadNum(int numThreads) {
        _numThreads = numThreads;
    }

    void Start(const ThreadInitCallback &cb = ThreadInitCallback());

    /**
     * 如果工作在多线程中，baseLoop_默认以轮询的方式分配Channel给subLoop
     * @return
     */
    EventLoop *GetNextLoop();

    /**
     * 返回事件循环池所有的EventLoop
     * @return
     */
    std::vector<EventLoop *> GetAllLoops();

    bool Started() const {
        return _started;
    }

    const std::string Name() const {
        return _name;
    }

private:
    EventLoop *_baseLoop;// 我们使用muduo编写程序的时候，就会定义一个EventLoop变量，这个变量作为TcpServer构造函数的参数，用户创建的就叫做baseLoop
    std::string _name;
    bool _started;
    int _numThreads;
    int _next;
    std::vector<std::unique_ptr<EventLoopThread>> _threads;
    std::vector<EventLoop *> _loops;
};


#endif //CMFNETLIB_EVENTLOOPTHREADPOOL_HPP
