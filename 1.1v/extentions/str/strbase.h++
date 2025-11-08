namespace str {
	std::vector<std::string> split(std::string str, std::string spliter) {
		std::vector<std::string> ret;
		std::size_t pos = 0;

		while ((pos = str.find(spliter)) != std::string::npos) {
		    std::string token = str.substr(0, pos);
		    if (!token.empty())
		        ret.push_back(token);
		    str.erase(0, pos + spliter.length());
		}
		if (!str.empty())
		    ret.push_back(str);
		return ret;
	}

	std::vector<std::string> split(std::string str, char spliter) {
		std::vector<std::string> ret;
		std::size_t pos = 0;
		while ((pos = str.find(spliter)) != std::string::npos) {
		    std::string token = str.substr(0, pos);
		    if (!token.empty())
		        ret.push_back(token);
		    str.erase(0, pos + 1);
		}
		if (!str.empty())
		    ret.push_back(str);
		return ret;
	}
}
