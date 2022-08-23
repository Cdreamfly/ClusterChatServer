//
// Created by Cmf on 2022/6/8.
//

#ifndef CMFNETLIB_THREAD_HPP
#define CMFNETLIB_THREAD_HPP

#include "base/noncopyable.hpp"
#include <functional>
#include <memory>
#include <thread>
#include <atomic>
#include <string>

class Thread : private noncopyable {
public:
    using ThreadFunc = std::function<void()>;
    using ptr = std::shared_ptr<Thread>;

    explicit Thread(ThreadFunc func, const std::string &name = std::string());

    ~Thread();

    void Start();

    void Join();

    bool Started() const {
        return _started;
    }

    std::thread::id Tid() const {
        return _tid;
    }

    const std::string &Name() const {
        return _name;
    }

    static int NumCreated() {
        return _numCreated;
    }

private:
    bool _started;//启动当前线程
    bool _joined;//当前线程等待其他线程完了再运行下去
    std::shared_ptr<std::thread> _thread;//自己来掌控线程对象产生的时机
    std::thread::id _tid;
    ThreadFunc _func;//存储线程函数
    std::string _name;//调试的时候打印
    static std::atomic_int _numCreated;//对线程数量计数
};


#endif //CMFNETLIB_THREAD_HPP
