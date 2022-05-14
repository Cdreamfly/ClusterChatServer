## C++ 集群聊天


### 通信格式
服务器和客户端的通信采用了JSON来完成数据在网络中的标准传输。
对于不同的数据则是采用了不同的格式，具体如下：
```
1.登录
json["msgid"] = LOGIN_MSG;
json["id"]			//用户id
json["password"]	//密码

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
json["password"]	//用户姓名

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
json["groupdesc"]	//群描述

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
```

### 问题
```
1.发送错误的json程序会挂掉但是异常退出处理程序没有执行，直接关闭窗口却可以。
2.群聊加群有问题：用户3已加入群但是登录是却没有返回正缺德json
3.群聊聊天问题：有时候能收到但是等待时间很长。
```