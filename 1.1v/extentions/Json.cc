module;
#include <nlohmann/json.hpp>
#undef __DEPRECATED
export module skbase.json;

import std;

export using json = nlohmann::json;

export namespace Json {
    json fromfile(std::string path);
    json fromfile(const char* path);
}

json Json::fromfile(std::string path) { return Json::fromfile(path.c_str()); }
json Json::fromfile(const char* path) {
	std::ifstream ifs;
	ifs.open(path, std::ios::in);
	std::string ret = "", line = "";
	while (std::getline(ifs, line)) ret += line + '\n';
	ifs.close();
	return json::parse(ret);
}
