namespace str {
	std::vector<std::wstring> split(std::wstring str, std::wstring spliter) {
		std::vector<std::wstring> ret;
		std::size_t pos = 0;

		while ((pos = str.find(spliter)) != std::wstring::npos) {
		    std::wstring token = str.substr(0, pos);
		    if (!token.empty())
		        ret.push_back(token);
		    str.erase(0, pos + spliter.length());
		}
		if (!str.empty())
		    ret.push_back(str);
		return ret;
	}

	std::vector<std::wstring> split(std::wstring str, wchar_t spliter) {
		std::vector<std::wstring> ret;
		std::size_t pos = 0;
		while ((pos = str.find(spliter)) != std::wstring::npos) {
		    std::wstring token = str.substr(0, pos);
		    if (!token.empty())
		        ret.push_back(token);
		    str.erase(0, pos + 1);
		}
		if (!str.empty())
		    ret.push_back(str);
		return ret;
	}
}
