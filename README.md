## C++ 集群聊天
### 后续
- 使用自己写的C++11 muduo 已完成
- 引入数据库连接池 已完成
- 序列化格式由JSON改为Protobuf
- nginx集群改为分布式集群
- 轮询可以改为更优负载均衡(轮询)，根据当前服务的负载量
- 数据库分库分表
- 新增业务
### 问题
1.发送错误的json程序会挂掉但是异常退出处理程序没有执行，直接关闭窗口却可以。

初步判断是**js["id"].get<int>()** json如果类型不是所要类型json就会报异常。
解决方法是在获取前加判断，

~~2.群聊加群有问题：用户3已加入群但是登录是却没有返回正缺德json~~
~~3.群聊聊天问题：有时候能收到但是等待时间很长~~
原因：添加群员函数参数写反了

~~4.无法获取离线消息。~~
发送消息我添加了双引号，去掉就可以了，我认为是json库的一个BUG，已经提交issus
给的回答是我的问题，双引号要转义

5.改为我用C++11重写的muduo网路库，路程艰辛，有变量名用错，段错误，链接错误，还有个奇怪的问题，传输一个json登录的都可以，就私聊的不可以，该形参类型const char*为string& 就好了？？？，原本推断是buffer的问题，后来觉得是输入字符编码问题？？不会吧。
### 编译
所需库引入
```
git clone https://github.com/redis/hiredis 从github上下载hiredis客户端，进行源码编译安装
cd hiredis
make
sudo make install
```
### 通信格式
服务器和客户端的通信采用了JSON来完成数据在网络中的标准传输。
对于不同的数据则是采用了不同的格式，具体如下：
```
1.登录
json["msgid"] = LOGIN_MSG;
json["id"]			//用户id
json["pwd"]	//密码

2.登录反馈
json["msgid"] = LOGIN_MSG_ACK;
json["id"]			//登录用户id
json["name"]		//登录用户密码
json["offlinemsg"]	//离线消息
json["friends"]		//好友信息,里面有id、name、state三个字段
json["groups"]		//群组信息,里面有id，groupname，groupdesc，users三个字段
					//users里面则有id，name，state，role四个字段
json["errno"]		//错误字段，错误时被设置成1，用户不在线设置成2
json["errmsg"]		//错误信息

3.注册
json["msgid"] = REG_MSG;
json["name"]		//用户姓名
json["pwd"]	//密码

4.注册反馈
json["msgid"] = REG_MSG_ACK;
json["id"]			//给用户返回他的id号
json["errno"]		//错误信息，失败会被设置为1

5.加好友
json["msgid"] = ADD_FRIEND_MSG;
json["id"]			//当前用户id
json["friendid"]	//要加的好友的id

6.一对一聊天
json["msgid"] = ONE_CHAT_MSG;
json["id"]			//发送者id
json["name"]		//发送者姓名
json["to"]			//接受者id
json["msg"]			//消息内容
json["time"]		//发送时间

7.创建群
json["msgid"] = CREATE_GROUP_MSG;
json["id"]			//群创建者id
json["groupname"]	//群名
json["groupdesc"]	//群描述 creator 创建者 normal 群员

8.加入群
json["msgid"] = ADD_GROUP_MSG;
json["id"]			//用户id
json["groupid"]		//群id

9.群聊
json["msgid"] = GROUP_CHAT_MSG;
json["id"]			//发送者id
json["name"]		//发送者姓名
json["groupid"]		//群id
json["msg"]			//消息内容
json["time"]		//发送时间

10.注销
json["msgid"] = LOGINOUT_MSG;
json["id"]			//要注销的id
```

### 测试json
```
登录
{"msgid":1,"id":3,"pwd":"123456"}

注册
{"msgid":4,"id":2,"pwd":"123456"}

发消息
{"msgid":6,"id":1,"from":"cmf","toid":2,"msg":" xxxxxxoooooo!"} 

加好友
{"msgid":7,"id":2,"friendid":1}

//创建群
{"msgid":8,"id":1,"groupname":"test","groupdesc":"test"}

//加入群
{"msgid":9,"id":3,"groupid":1}

//群聊天
{"msgid":10,"id":1,"name":"mmm","groupid":1,"msg":"hello every one"}

//注销
{"msgid":3,"id":1}
```

## nginx负载均衡
我们客户端的请求先发的nginx，nginx根据配置既定的负载均衡算法，把第一个请求转到第一台服务器，根据权重相等，轮询，下一个客户端连接到第二台服务器。
### nginx 安装
```
nginx编译安装需要先安装pcre、openssl、zlib等库
进入源码后执行安装需要加入–with-stream参数来激活tcp负载均衡模块。
./configure --with-stream 
make && make install


```
### nginx配置
```
cd /usr/local/nginx/conf/nginx.conf
```
```
events {
    worker_connections  1024;
}

stream {
    upstream MyServer{
#server id:port 权重都为1（设,2:1 三次链接两次为2，一次为1）超过三次心跳失败就认为挂掉了，等待心跳时间30s
       server 127.0.0.1:9190 weight=1 max_fails=3 fail_timeout=30s;
       server 127.0.0.1:9192 weight=1 max_fails=3 fail_timeout=30s;
     }

     server {
#nginx连接服务器超过1s判定失败
        proxy_connect_timeout 1s;
        #proxy_timeout 3s;
        #监听客户端8000端口
        listen 8000;
        proxy_pass MyServer;
        tcp_nodelay on;
   }
}
```