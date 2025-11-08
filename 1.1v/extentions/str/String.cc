export module skbase.str.String;

import skbase.json;
import skbase.memory.array;
import skbase.geomath.math;
import std;

#include "../../defines.h++"

#define TXT_TO_NUM \
math::Number::to_num

export
#include "strbase.h++"



export namespace str {
    using string = std::string;

    class String : public string {
    protected:
        void deserialize(const json& j) { this->assign(j["data"].get<string>()); }
        void serialize(json& j) { j["data"] = *this; }
    public :
        String(const char* src= "") :string(src) {}
        String(string src)          :string(src) {}
        using std::string::string;

        //        take a cut
        inline String cut(int start, int end) { return this->substr(start, end); }
        template <long int s_num=0, long int e_num=0>
        inline String cut(const String& s, const String& e) {
			std::size_t si;
			if constexpr (s_num == -1) si= this->lastOf        (s) + s.length();
			else                       si= this->indexOf<s_num>(s) + s.length();
			std::size_t ei;
			if constexpr (e_num == -1) ei= this->lastOf        (e) - si;
			else                       ei= this->indexOf<e_num>(e) - si;
			return this->cut(si, ei);
        }

        inline bool end_with(const String& prefix) { return (contains(prefix) && indexOf(prefix) == int(size() - prefix.size())); }
        inline bool start_with(const String& prefix, int toffset = 0) { return (contains(prefix) && indexOf(prefix) == toffset); }
        inline bool end_with(const String& prefix, int toffset) { return (contains(prefix) && lastOf(prefix) == toffset); }
        inline bool contains(const String& subStr) { return (this->find(subStr) != std::string::npos); }
        inline bool contains(const char& subStr) { return (this->find(subStr) != std::string::npos); }

        template <bool is_end=false>
        inline String without(std::size_t i=0) {
			if constexpr (is_end)
				return this->cut(i, this->length()-1);
			else
				return this->cut(0, this->length()-1-i);
        }
        template <char is_end='s', long int num=0>
        inline String without(const String& mark) {
			if constexpr (is_end == 'e') {
				if constexpr (num == -1)
					return this->cut(this->lastOf(mark)+mark.length(), this->length()-1);
				else
					return this->cut(this->indexOf<num>(mark)+mark.length(), this->length()-1);
			} else {
				if constexpr (num == -1)
					return this->cut(0, this->lastOf(mark));
				else
					return this->cut(0, this->indexOf<num>(mark));
			}
        }
        template <char is_end='s', long int num=0>
        inline String without(const char& mark) {
			if constexpr (is_end == 'e') {
				if constexpr (num == -1)
					return this->cut(this->lastOf(mark)+1, this->length()-1);
				else
					return this->cut(this->indexOf<num>(mark)+1, this->length()-1);
			} else {
				if constexpr (num == -1)
					return this->cut(0, this->lastOf(mark));
				else
					return this->cut(0, this->indexOf<num>(mark));
			}
        }

        inline String last (std::size_t length_=1) { return cut(length() - length_, length()); }
        inline String first(std::size_t length_=1) { return cut(0, length_); }

        template<std::size_t times=1>
        String replace(const String& from, const String& to) {
            if constexpr (times == 1) {
                string ret = *this;
                std::size_t pos = 0;
                pos = ret.find(from, pos);
                if (pos != std::string::npos) ret.replace(pos, from.length(), to);
                return ret;
            } else {
                string ret = *this;
                std::size_t pos = 0;
                for (std::size_t i=0;(pos = ret.find(from, pos)) != std::string::npos && i < times;i++) {
                    ret.replace(pos, from.length(), to);
                    pos += to.length();
                }
                return ret;
            }
        }

        String replace_all(const String& from, const String& to);

        String delete_(const String& from) { return replace(from, ""); }

        String delete_all(const String& from) { return this->replace_all(from, ""); }

        inline bool cmp(const String& str) { return this->compare(str); }

        Arrays::Array<String> split(String spliter) {
            Arrays::Array<String> ret;
            std::size_t pos = 0;
            while ((pos = this->find(spliter)) != String::npos) {
                String token = this->substr(0, pos);
                if (!token.empty())
                    ret.push_back(token);
                this->erase(0, pos + 1);
            }
            if (!this->empty())
                ret.push_back("");
            return ret;
        }
        Arrays::Array<String> split(char spliter) {
            Arrays::Array<String> ret;
            std::size_t pos = 0;
            while ((pos = this->find(spliter)) != String::npos) {
                String token = this->substr(0, pos);
                if (!token.empty())
                    ret.push_back(token);
                this->erase(0, pos + 1);
            }
            if (!this->empty())
                ret.push_back("");
            return ret;
        }

        Arrays::Array<String> splitTo(int number);

        inline bool containsAll(std::vector<std::string> subStr);
        inline bool containsAll(const String* subStr);
        inline bool containsAll(std::vector<char> subStr);
        inline bool containsAll(const char* subStr);

        inline bool containsPieces(const String subStr) { return this->containsPieces(subStr.c_str()); }
        inline bool containsPieces(std::vector<std::string> subStr);
        inline bool containsPieces(const String* subStr);

        inline bool containsOneOf(std::vector<std::string> subStr);
        inline bool containsOneOf(const String* subStr);
        inline bool containsOneOf(std::vector<char> subStr);
        inline bool containsOneOf(const char* subStr);

        std::size_t numberOf(const String& splinter);
        std::size_t numberOf(char splinter);

        std::size_t firstOf(const String& index);
        std::size_t lastOf(const String& index);
        std::size_t firstOf(char index);
        std::size_t lastOf(char index);

        template<std::size_t index_num=0>
        std::size_t indexOf(const String& value_) {
            if constexpr (index_num > 0) {
                String str= *this;
                std::size_t ret = str.find(value_, index_num);
                if (ret == std::string::npos)
					std:: cerr	<< "\e[1;31mError\e[0m: the value that you give is no found in this String\n"
								<< "\tthat 'indexOf<std::size_t index_num="<<index_num<<">(const item_t& value_) function'\n"
								<< "\twill return 'std::string::npos=\""<<std::string::npos<<"\"' that may make same errors in use\n";
				return ret;
            } else {
                std::size_t ret = this->find(value_);
                if (ret == std::string::npos)
					std:: cerr	<< "\e[1;31mError\e[0m: the value that you give is no found in this String\n"
								<< "\tthat 'indexOf<std::size_t index_num="<<index_num<<">(const item_t& value_) function'\n"
								<< "\twill return 'std::string::npos=\""<<std::string::npos<<"\"' that may make same errors in use\n";
		        return ret;
		    }
        }
        template<std::size_t index_num=0>
        std::size_t indexOf(const char& value_) {
            if constexpr (index_num > 0) {
                String str= *this;
                std::size_t ret = str.find(value_, index_num);
                if (ret == std::string::npos)
					std:: cerr	<< "\e[1;31mError\e[0m: the value that you give is no found in this String\n"
								<< "\tthat 'indexOf<std::size_t index_num="<<index_num<<">(const item_t& value_) function'\n"
								<< "\twill return 'std::string::npos=\""<<std::string::npos<<"\"' that may make same errors in use\n";
				return ret;
            } else {
                std::size_t ret = this->find(value_);
                if (ret == std::string::npos)
					std:: cerr	<< "\e[1;31mError\e[0m: the value that you give is no found in this String\n"
								<< "\tthat 'indexOf<std::size_t index_num="<<index_num<<">(const item_t& value_) function'\n"
								<< "\twill return 'std::string::npos=\""<<std::string::npos<<"\"' that may make same errors in use\n";
		        return ret;
		    }
        }

        String trim();

        String pad_right(std::size_t total_width, char padding_char = ' ');
        String pad_left(std::size_t total_width, char padding_char = ' ');
        String reverse();

        inline String operator ()(int start, int end) { return cut(start, end); }
        inline char operator [] (int number) { return this->c_str()[number]; }

        inline String operator - (const String& str) { return String(replace_all(str, "")); }
        inline void operator -= (const String& str) { this->assign(replace_all(str, "")); }

        inline unsigned int touint() { return TXT_TO_NUM<unsigned int>(*this); }
        inline float tofloat() { return TXT_TO_NUM<float>(*this); }
        inline int toint() { return TXT_TO_NUM<int>(*this); }

		template<typename num_t> requires std::is_arithmetic_v<num_t>
        num_t tonum() { return TXT_TO_NUM<num_t>(*this); }

        // operators
        inline String operator * (std::size_t indexs) {
            String ret = *this;
            for (std::size_t i = 0; i < indexs; i++) ret += ret;
            return ret;
        }
        inline void operator *=(std::size_t indexs)
            { for (std::size_t i = 0; i < indexs; i++) this->operator+=(*this); }
    };
}

export using str_t = str::String;
