//
// Created by Cmf on 2022/5/15.
//

#ifndef CLUSTERCHAT_REDIS_H
#define CLUSTERCHAT_REDIS_H

#include <functional>
#include <hiredis/hiredis.h>

class Redis
{
public:
    Redis();
    ~Redis();
    //连接redis服务器
    bool Connect();
    //向redis指定的通道channel发布消息
    bool Publish(int channel,std::string msg);
    //向redis指定的通道subscribe订阅消息
    bool Subscribe(int channel);
    //向redis指定的通道unsubscribe取消订阅消息
    bool UnSubscribe(int channel);
    //在独立线程中接收订阅通道中的消息
    void ObserverChannelMessage();
    //初始化向业务层上报通道消息的回调对象
    void InitNotifyHandler(std::function<void(int, std::string)> fn);

private:
    redisContext*_publishContext;   //hiredis同步上下文对象，负责publish消息，相当于客户端一个redis-client
    redisContext*_subcribeContext;  //hiredis同步上下文对象，负责subscribe消息，相当于客户端一个redis-client,_publish和_subcribe不能在一个上下文处理 ,会阻塞
    std::function<void(int,std::string)>_notifyMessageHandler;    //回调操作，收到订阅的消息，给service层上报
};

#endif //CLUSTERCHAT_REDIS_H
