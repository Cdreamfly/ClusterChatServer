## V1

### 数据库
```shell
sudo apt install libmysqlclient-dev
```
```sql
CREATE DATABASE IF NOT EXISTS chat;
```
```shell
mariadb -u root -p chat < chat.sql
```
### 通信协议
```json
// 1.登录
json["msgId"] = LOGIN_MSG;
json["id"]			//用户id
json["pwd"]	//密码

// 2.登录反馈
json["msgId"] = LOGIN_MSG_ACK;
json["id"]			//登录用户id
json["name"]		//登录用户密码
json["offlineMsg"]	//离线消息
json["friends"]		//好友信息,里面有id、name、state三个字段
json["groups"]		//群组信息,里面有id，groupname，groupdesc，users三个字段
					//users里面则有id，name，state，role四个字段
json["errno"]		//错误字段，错误时被设置成1，用户不在线设置成2
json["errmsg"]		//错误信息

// 3.注册
json["msgId"] = REG_MSG;
json["name"]		//用户姓名
json["pwd"]	//密码

// 4.注册反馈
json["msgId"] = REG_MSG_ACK;
json["id"]			//给用户返回他的id号
json["errno"]		//错误信息，失败会被设置为1

// 5.加好友
json["msgId"] = ADD_FRIEND_MSG;
json["id"]			//当前用户id
json["friendId"]	//要加的好友的id

// 6.一对一聊天
json["msgId"] = ONE_CHAT_MSG;
json["id"]			//发送者id
json["name"]		//发送者姓名
json["to"]			//接受者id
json["msg"]			//消息内容
json["time"]		//发送时间

// 7.创建群
json["msgId"] = CREATE_GROUP_MSG;
json["id"]			//群创建者id
json["groupName"]	//群名
json["groupDesc"]	//群描述

// 8.加入群
json["msgId"] = ADD_GROUP_MSG;
json["id"]			//用户id
json["groupId"]		//群id

// 9.群聊
json["msgId"] = GROUP_CHAT_MSG;
json["id"]			//发送者id
json["name"]		//发送者姓名
json["groupId"]		//群id
json["msg"]			//消息内容
json["time"]		//发送时间

// 10.注销
json["msgId"] = LOGINOUT_MSG;
json["id"]			//要注销的id
```
### TEST
```json
// 登录
{"msgId":1,"id":3,"pwd":"123456"}

// 注册
{"msgId":4,"id":2,"pwd":"123456"}

// 发消息
{"msgId":6,"id":1,"from":"cmf","toId":2,"msg":" xxxxxxoooooo!"} 

// 加好友
{"msgId":7,"id":2,"friendId":1}

// 创建群
{"msgId":8,"id":1,"groupName":"test","groupDesc":"test"}

// 加入群
{"msgId":9,"id":3,"groupId":1}

// 群聊天
{"msgId":10,"id":1,"name":"mmm","groupId":1,"msg":"hello every one"}

// 注销
{"msgId":3,"id":1}
```