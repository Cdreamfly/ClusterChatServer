#include <fstream>
#include <iostream>
#include "chatServer.hpp"
#include "cm/net/EventLoop.hpp"
#include "db/dbConf.hpp"
#include "chatService.hpp"
#include "db/connectionManage.hpp"

bool dbTest() {
	std::ifstream ifs("../dbConf.json");
	if (!ifs.is_open()) {
		return false;
	}
	nlohmann::json js;
	ifs >> js;
	DbConf dbConf = js;
	json res = dbConf;
	std::cout << res.dump() << std::endl;
	return true;
}

int main() {
	ConnectionManage::getConnectionManage().getConnection().showTasks("select * from user");
	//dbTest();
	/*
	cm::net::EventLoop loop;
	cm::net::InetAddress addr(8181);
	ChatServer server(&loop, addr, "chatServer");
	server.start();
	loop.loop();
	 */
	return 0;
}