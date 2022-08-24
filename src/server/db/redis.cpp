//
// Created by Cmf on 2022/5/15.
//
#include "server/db/redis.h"
#include <iostream>
#include <thread>

Redis::Redis() : _publishContext(nullptr), _subcribeContext(nullptr) {}

Redis::~Redis() {
    if (_publishContext != nullptr) {
        redisFree(_publishContext);
    }
    if (_subcribeContext != nullptr) {
        redisFree(_subcribeContext);
    }
}

bool Redis::Connect() {
    _publishContext = redisConnect("127.0.0.1", 6379);
    if (_publishContext == nullptr) {
        std::cerr << "connect redis failed!" << std::endl;
        return false;
    }
    _subcribeContext = redisConnect("127.0.0.1", 6379);
    if (_subcribeContext == nullptr) {
        std::cerr << "connect redis failed!" << std::endl;
        return false;
    }
    //在单独的线程中，监听通道上的事件，有消息给业务层进行上报
    std::thread t([&] {
        ObserverChannelMessage();
    });
    t.detach();
    std::cout << "connect redis-server success!" << std::endl;
    return true;
}

/*
 * 向redis指定的通道channel发布消息,redisCommand先把命令缓存在本地，然后把命令发送到redis-server，然后阻塞等待命令的执行结果
 */
bool Redis::Publish(int channel, std::string msg) {
    redisReply *reply = static_cast<redisReply *>(redisCommand(_publishContext, "PUBLISH %d %s", channel, msg.c_str()));
    if (reply == nullptr) {
        std::cerr << "publish command failed!" << std::endl;
        return false;
    }
    freeReplyObject(reply);
    return false;
}

/*
 * 向redis指定的通道subscribe订阅消息
 */
bool Redis::Subscribe(int channel) {
    /*
     * SUBSCRIBE命令本身会造成线程阻塞等待通道里面发生消息，这里只做订阅通道，不接收通道消息
     * 通道消息的接收专门在observer_channel_message函数中的独立线程中进行
     * 只负责发送命令，不阻塞接收redis server响应消息，否则和notifyMsg线程抢占响应资源
     */
    if (REDIS_ERR == redisAppendCommand(_subcribeContext, "SUBSCRIBE %d", channel)) {
        std::cerr << "subscribe command failed!" << std::endl;
        return false;
    }
    //redisBufferWrite可以循环发送缓冲区，直到缓冲区数据发送完毕（done被置为1）
    int done = 0;
    while (!done) {
        if (REDIS_ERR == redisBufferWrite(_subcribeContext, &done)) {
            std::cerr << "subscribe command failed!" << std::endl;
            return false;
        }
    }
    return true;
}

bool Redis::UnSubscribe(int channel) {
    if (REDIS_ERR == redisAppendCommand(_subcribeContext, "UNSUBSCRIBE %d", channel)) {
        std::cerr << "unsubscribe command failed!" << std::endl;
        return false;
    }
    int done = 0;
    while (!done) {
        if (REDIS_ERR == redisBufferWrite(_subcribeContext, &done)) {
            std::cerr << "unsubscribe command failed!" << std::endl;
            return false;
        }
    }
    return true;
}

/*
 * 在独立线程中接收订阅通道中的消息
 */
void Redis::ObserverChannelMessage() {
    redisReply *reply = nullptr;
    while (REDIS_OK == redisGetReply(_subcribeContext, (void **) &reply)) {
        //订阅收到的消息是一个带三元素的数组
        if (reply != nullptr && reply->element[2] != nullptr && reply->element[2]->str != nullptr) {
            _notifyMessageHandler(atoi(reply->element[1]->str), reply->element[2]->str);
        }
        freeReplyObject(reply);
    }
    std::cerr << ">>>>>>>>>>>>> observer_channel_message quit <<<<<<<<<<<<<" << std::endl;
}

void Redis::InitNotifyHandler(std::function<void(int, std::string)> fn) {
    _notifyMessageHandler = fn;
}




