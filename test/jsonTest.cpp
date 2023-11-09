#include "json.hpp"
#include <iostream>

using json = nlohmann::json;

void jsonTest() {
	json js;
	js["name"] = "cmf";
	js["age"] = "19";
	std::cout << js.dump() << std::endl;
}