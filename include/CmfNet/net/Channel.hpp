//
// Created by Cmf on 2022/6/2.
//

#ifndef CMFNETLIB_CHANNEL_HPP
#define CMFNETLIB_CHANNEL_HPP

#include <memory>
#include <functional>
#include <sys/epoll.h>
#include "base/Timestamp.hpp"
#include "base/Log.hpp"


/**
 * Channel(事件分发器),只属于一个EventLoop,Channel类中保存着IO事件的类型以及对应的回调函数,每个channel只负责一个文件描述符
 */
class EventLoop;

class Channel : private noncopyable {
public:
    using ptr = std::shared_ptr<Channel>;
    using EventCallback = std::function<void()>;    //事件回调
    using ReadEventCallback = std::function<void(Timestamp)>;//读事件回调


    Channel(EventLoop *loop, int fd) : _loop(loop), _fd(fd), _events(0), _revents(0), _index(-1), _tied(false) {
    }

    /**
     * 设置回调函数对象
     * @param cb
     */
    void SetReadCallback(ReadEventCallback cb) {
        this->_readCallback = std::move(cb);
    }

    void SetWriteCallback(EventCallback cb) {
        this->_writeCallback = std::move(cb);
    }

    void SetCloseCallback(EventCallback cb) {
        this->_closeCallback = std::move(cb);
    }

    void SetErrorCallback(EventCallback cb) {
        this->_errorCallback = std::move(cb);
    }

    /**
     * 返回fd当前的事件状态
     */
    bool IsNoneEvent() const {
        return this->_events == this->NoneEvent;
    }

    bool IsWriting() const {
        return this->_events & this->WriteEvent;
    }

    bool IsReading() const {
        return this->_events & this->ReadEvent;
    }

    /**
     * 设置fd相应的事件状态，要让fd对这个事件感兴趣,update就是调用epoll_ctrl，通知poller把fd感兴趣的事件添加到fd上
     */
    void EnableReading();

    void DisableReading();

    void EnableWriting();

    void DisableWriting();

    void DisableAll();

    uint32_t Revents() const {
        return this->_revents;
    }

    void SetRevents(uint32_t revt) {
        this->_revents = revt;
    }

    int Index() const {
        return this->_index;
    }

    void SetIndex(int index) {
        this->_index = index;
    }

    uint32_t Events() const {
        return this->_events;
    }

    int Fd() const {
        return this->_fd;
    }

    void Remove();

    /**
     * 防止当channel被手动remove掉，channel还在执行回调操作
     * @param obj
     */
    void Tie(const std::shared_ptr<void> &obj) {
        this->_tie = obj;
        this->_tied = true;
    }

    /**
     * 处理事件
     * @param receiveTime
     */
    void HandleEvent(Timestamp receiveTime) {
        if (this->_tied) {
            std::shared_ptr<void> guard = this->_tie.lock();
            if (guard) {
                this->HandleEventWithGuard(receiveTime);
            }
        } else {
            this->HandleEventWithGuard(receiveTime);
        }
    }


private:
    /**
     * //根据poller通知的channel发生的具体事件， 由channel负责调用具体的回调操作
     * EPOLLIN:表示对应的文件描述符可以读；
     * EPOLLOUT:表示对应的文件描述符可以写；
     * EPOLLPRI:表示对应的文件描述符有紧急的数据可读
     * EPOLLERR:表示对应的文件描述符发生错误；
     * EPOLLHUP:表示对应的文件描述符被挂断；
     * EPOLLET:表示对应的文件描述符有事件发生；
     * @param receiveTime
     */
    void HandleEventWithGuard(Timestamp receiveTime) {
        LOG_INFO("channel handleEvent revents:%d", Revents());
        if ((this->_revents & EPOLLHUP) && !(this->_revents & EPOLLIN)) {
            if (this->_closeCallback) {
                this->_closeCallback();
            }
        }
        if (this->_revents & EPOLLERR) {
            if (this->_errorCallback) {
                this->_errorCallback();
            }
        }
        if (this->_revents & (EPOLLIN | EPOLLPRI)) {
            if (this->_readCallback) {
                this->_readCallback(receiveTime);
            }
        }
        if (this->_revents & EPOLLOUT) {
            if (this->_writeCallback) {
                this->_writeCallback();
            }
        }
    }

private:


    static const int NoneEvent = 0; //无事件
    static const int ReadEvent = EPOLLIN | EPOLLET; //可读事件
    static const int WriteEvent = EPOLLOUT; //可写事件

    EventLoop *_loop;   //channel所属的loop,一个channel只属于一个loop
    const int _fd;          //channel所属的文件描述符
    uint32_t _events;       //注册的事件
    uint32_t _revents;      //poller设置的就绪的事件
    int _index;             //被poller使用的下标
    std::weak_ptr<void> _tie;
    bool _tied;

    ReadEventCallback _readCallback;//读事件回调
    EventCallback _writeCallback;   //写事件回调
    EventCallback _closeCallback;   //关闭事件回调
    EventCallback _errorCallback;   //错误事件回调
};

#endif //CMFNETLIB_CHANNEL_HPP
