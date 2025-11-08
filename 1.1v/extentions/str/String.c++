module skbase.str.String;

str::String str::String::replace_all(const String& from, const String& to) {
    string ret = *this;
    std::size_t pos = 0;
    while ((pos = ret.find(from, pos)) != std::string::npos) {
        ret.replace(pos, from.length(), to);
        pos += to.length();
    }
    return ret;
}

Arrays::Array<str::String> str::String::splitTo(int number) {
    Arrays::Array<str::String> ret = Arrays::Array<str::String>();
    int val = int(this->length()) / number;
    for (int i = 0; i < number; i++) {
        ret.add(String(""));
        for (int j = 0; j < val; j++) ret[i] += this[j + val * i];
    } return ret;
}

bool str::String::containsAll(std::vector<std::string> subStr) {
    for (std::size_t i=0, size=subStr.size(); i<size; i++) if (!contains(subStr[i])) return false;
    return true;
}
bool str::String::containsAll(const String* subStr) {
    for (std::size_t i=0, size=(sizeof(subStr)/sizeof(std::string)); i<size; i++) if (!contains(subStr[i])) return false;
    return true;
}
bool str::String::containsAll(std::vector<char> subStr) {
    for (std::size_t i=0, size=subStr.size(); i<size; i++) if (!contains(subStr[i])) return false;
    return true;
}
bool str::String::containsAll(const char* subStr) {
    for (std::size_t i=0, size=(sizeof(subStr)/sizeof(char)); i<size; i++) if (!contains(subStr[i])) return false;
    return true;
}


bool str::String::containsPieces(std::vector<std::string> subStr) {
    for (std::string txt : subStr) if (!containsAll(txt.c_str())) return false;
    return true;
}
bool str::String::containsPieces(const String* subStr) {
    for (std::size_t i=0, size=(sizeof(subStr)/sizeof(std::string)); i<size; i++) if (!containsAll(subStr[i].c_str())) return false;
    return true;
}


bool str::String::containsOneOf(const String* subStr) {
    for (std::size_t i=0, size=(sizeof(subStr)/sizeof(std::string)); i<size; i++) if (contains(subStr[i])) return true;
    return false;
}
bool str::String::containsOneOf(std::vector<std::string> subStr) {
    for (std::size_t i=0, size=subStr.size(); i<size; i++) if (contains(subStr[i])) return true;
    return false;
}
bool str::String::containsOneOf(const char* subStr) {
    for (std::size_t i=0, size=(sizeof(subStr)/sizeof(char)); i<size; i++) if (contains(subStr[i])) return true;
    return false;
}
bool str::String::containsOneOf(std::vector<char> subStr) {
    for (std::size_t i=0, size=subStr.size(); i<size; i++) if (contains(subStr[i])) return true;
    return false;
}

std::size_t str::String::firstOf(const String& index)
    { return this->find_first_of(index); }
std::size_t str::String::firstOf(char index)
    { return this->find_first_of(index); }

std::size_t str::String::lastOf(char index)
    { return this->find_last_of(index); }
std::size_t str::String::lastOf(const String& index)
    { return this->find_last_of(index); }
std::size_t str::String::numberOf(char splinter) {
    if (this->contains(splinter + "")) return std::size_t(split(splinter).size() - 1);
    else return 0;
}
std::size_t str::String::numberOf(const String& splinter) {
    if (this->contains(splinter)) return std::size_t(split(splinter).size() - 1);
    else return 0;
}

str::String str::String::trim() {
    auto start = find_first_not_of(" \t\n\r\f\v");
    auto end = find_last_not_of(" \t\n\r\f\v");
    if (start == npos) clear();
    else *this = substr(start, end - start + 1);
    return *this;
}



str::String str::String::reverse() {
    string ret = *this;
    std::reverse(ret.begin(), ret.end());
    return ret;
}
str::String str::String::pad_left(std::size_t total_width, char padding_char) {
    str::String ret = *this;
    if (total_width > ret.length()) insert(0, total_width - ret.length(), padding_char);
    return ret;
}
str::String str::String::pad_right(std::size_t total_width, char padding_char) {
    str::String ret = *this;
    if (total_width > ret.length()) ret.append(total_width - ret.length(), padding_char);
    return ret;
}
