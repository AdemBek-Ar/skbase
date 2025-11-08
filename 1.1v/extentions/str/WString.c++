module skbase.str.WString;

str::WString str::WString::replace_all(const std::wstring& from, const std::wstring& to) {
    wstring str = *this;
    int sizeof_loop = int(split(from).size() - 1);
    for (int i = 0; i < sizeof_loop; i++) {
        std::size_t start_pos = str.find(from);
        if (start_pos != std::wstring::npos) str.replace(start_pos, from.length(), to);
    } return WString(str);
}

Arrays::Array<str::WString> str::WString::splitTo(int number) {
    Arrays::Array<str::WString> ret = Arrays::Array<str::WString>();
    int val = int(this->length()) / number;
    for (int i = 0; i < number; i++) {
        ret.add(WString(L""));
        for (int j = 0; j < val; j++) ret[i] += this[j + val * i];
    } return ret;
}



bool str::WString::containsAll(std::vector<std::wstring> subStr) {
    for (std::size_t i=0, size=subStr.size(); i<size; i++) if (!contains(subStr[i])) return false;
    return true;
}
bool str::WString::containsAll(const std::wstring* subStr) {
    for (std::size_t i=0, size=(sizeof(subStr)/sizeof(std::wstring)); i<size; i++) if (!contains(subStr[i])) return false;
    return true;
}
bool str::WString::containsAll(std::vector<wchar_t> subStr) {
    for (std::size_t i=0, size=subStr.size(); i<size; i++) if (!contains(subStr[i])) return false;
    return true;
}
bool str::WString::containsAll(const wchar_t* subStr) {
    for (std::size_t i=0, size=(sizeof(subStr)/sizeof(wchar_t)); i<size; i++) if (!contains(subStr[i])) return false;
    return true;
}


bool str::WString::containsPieces(std::vector<std::wstring> subStr) {
    for (std::wstring txt : subStr) if (!containsAll(txt.c_str())) return false;
    return true;
}
bool str::WString::containsPieces(const std::wstring* subStr) {
    for (std::size_t i=0, size=(sizeof(subStr)/sizeof(std::wstring)); i<size; i++) if (!containsAll(subStr[i].c_str())) return false;
    return true;
}


bool str::WString::containsOneOf(const std::wstring* subStr) {
    for (std::size_t i=0, size=(sizeof(subStr)/sizeof(std::wstring)); i<size; i++) if (contains(subStr[i])) return true;
    return false;
}
bool str::WString::containsOneOf(std::vector<std::wstring> subStr) {
    for (std::size_t i=0, size=subStr.size(); i<size; i++) if (contains(subStr[i])) return true;
    return false;
}
bool str::WString::containsOneOf(const wchar_t* subStr) {
    for (std::size_t i=0, size=(sizeof(subStr)/sizeof(wchar_t)); i<size; i++) if (contains(subStr[i])) return true;
    return false;
}
bool str::WString::containsOneOf(std::vector<wchar_t> subStr) {
    for (std::size_t i=0, size=subStr.size(); i<size; i++) if (contains(subStr[i])) return true;
    return false;
}



std::size_t str::WString::firstOf(wstring index) {
    std::size_t pos = this->find_first_of(index);
    return (pos != wstring::npos) ? pos : -1;
}
std::size_t str::WString::firstOf(char index) {
    std::size_t pos = this->find_first_of(index);
    return (pos != wstring::npos) ? pos : -1;
}
std::size_t str::WString::indexOf(char index) {
    std::size_t pos = this->find(index);
    return (pos != wstring::npos) ? pos : -1;
}
std::size_t str::WString::indexOf(wstring index) {
    std::size_t pos = this->find(index);
    return (pos != wstring::npos) ? pos : -1;
}
std::size_t str::WString::indexOf(char index, std::size_t number) {
    std::size_t ret = -1;
    WString str = *this;
    for (std::size_t i = 0; i < number; i++) str = str.replace(index + L"", L"");
    return ret;
}
std::size_t str::WString::indexOf(wstring index, std::size_t number) {
    std::size_t ret = -1;
    WString str = *this;
    for (std::size_t i = 0; i < number; i++) str = str.replace(index, L"");
    return ret;
}
std::size_t str::WString::lastOf(char index) {
    std::size_t pos = this->find_last_of(index);
    return (pos != wstring::npos) ? pos : -1;
}
std::size_t str::WString::lastOf(wstring index) {
    std::size_t pos = this->find_last_of(index);
    return (pos != wstring::npos) ? pos : -1;
}
std::size_t str::WString::numberOf(char splinter) {
    if (this->contains(splinter + L"")) return std::size_t(split(splinter).size() - 1);
    else return 0;
}
std::size_t str::WString::numberOf(wstring splinter) {
    if (this->contains(splinter)) return std::size_t(split(splinter).size() - 1);
    else return 0;
}

str::WString str::WString::trim() {
    auto start = find_first_not_of(L" \t\n\r\f\v");
    auto end = find_last_not_of(L" \t\n\r\f\v");
    if (start == npos) clear();
    else *this = substr(start, end - start + 1);
    return *this;
}

str::WString str::WString::reverse() {
    wstring ret = *this;
    std::reverse(ret.begin(), ret.end());
    return ret;
}
str::WString str::WString::pad_left(std::size_t total_width, char padding_char) {
    str::WString ret = *this;
    if (total_width > ret.length()) insert(0, total_width - ret.length(), padding_char);
    return ret;
}
str::WString str::WString::pad_right(std::size_t total_width, char padding_char) {
    str::WString ret = *this;
    if (total_width > ret.length()) ret.append(total_width - ret.length(), padding_char);
    return ret;
}
