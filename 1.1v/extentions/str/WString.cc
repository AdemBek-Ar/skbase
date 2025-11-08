export module skbase.str.WString;

import skbase.json;
import skbase.memory.array;
import skbase.geomath.math;
import std;

#include "../../defines.h++"

#define TXT_TO_NUM \
math::Number::to_num

export
#include "wstrbase.h++"



export namespace str {
    using wstring = std::wstring;

    class WString : public wstring {
    protected:
        void serialize(json& j) { j["data"] = *this; }
        void deserialize(const json& j) { this->assign(j["data"].get<wstring>()); }
    public :
        WString(const wchar_t* src= L"") :wstring(src) {}
        WString(wstring src)             :wstring(src) {}
        using std::wstring::wstring;

        //        take a cut
        inline WString subwstringII(wstring start, int is, wstring end, int ie) { return this->substr(indexOf(start, is) + start.length(), indexOf(end, ie) - (start.length() + 1)); }
        inline WString subwstringIF(wstring start, int i, wstring end) { return this->substr(indexOf(start, i) + start.length(), firstOf(end) - indexOf(start, i) - 1); }
        inline WString subwstringIL(wstring start, int i, wstring end) { return this->substr(indexOf(start, i) + start.length(), lastOf(end) - indexOf(start, i) - 1); }
        inline WString subwstringFI(wstring start, wstring end, int i) { return this->substr(firstOf(start) + start.length(), indexOf(end, i) - firstOf(start) - 1); }
        inline WString subwstringLI(wstring start, wstring end, int i) { return this->substr(lastOf(start) + start.length(), indexOf(end, i) - lastOf(start) - 1); }
        inline WString subwstringI(wstring start, int i, int end) { return this->substr(indexOf(start, i) + start.length(), end - (indexOf(start, i) + 1)); }
        inline WString subwstring(wstring start, wstring end) { return this->substr(indexOf(start) + start.length(), indexOf(end) - (start.length() + 1)); }
        inline WString subwstringFF(wstring start, wstring end) { return this->substr(firstOf(start) + start.length(), firstOf(end) - firstOf(start) - 1); }
        inline WString subwstringFL(wstring start, wstring end) { return this->substr(firstOf(start) + start.length(), lastOf(end) - firstOf(start) - 1); }
        inline WString subwstringLL(wstring start, wstring end) { return this->substr(lastOf(start) + start.length(), lastOf(end) - lastOf(start) - 1); }
        inline WString subwstringL(wstring start, int end) { return this->substr(firstOf(start) + start.length(), end - (firstOf(start) + 1)); }
        inline WString subwstring(wstring start, int end) { return this->substr(indexOf(start) + start.length(), end - (indexOf(start) + 1)); }
        inline WString subwstringF(wstring start, int end) { return this->substr(lastOf(start) + start.length(), end - (lastOf(start) + 1)); }
        inline WString subwstringLF(wstring start, wstring end) { return this->substr(lastOf(start) + start.length(), firstOf(end)); }
        inline WString subwstringI(int start, wstring end, int i) { return this->substr(start, indexOf(end, i) + 1 - start); }
        inline WString subwstringF(int start, wstring end) { return this->substr(start, lastOf(end) + 1 - start); }
        inline WString subwstringL(int start, wstring end) { return this->substr(start, firstOf(end) - start); }
        inline WString subwstring(int start, wstring end) { return this->substr(start, indexOf(end) - start); }
        inline WString subwstring(int start, int end) { return this->substr(start, end); }

        inline bool end_with(const wstring& prefix) { return (contains(prefix) && indexOf(prefix) == int(size() - prefix.size())); }
        inline bool start_with(const wstring& prefix, int toffset = 0) { return (contains(prefix) && indexOf(prefix) == toffset); }
        inline bool end_with(const wstring& prefix, int toffset) { return (contains(prefix) && lastOf(prefix) == toffset); }
        inline bool contains(const std::wstring& subStr) { return (this->find(subStr) != std::wstring::npos); }
        inline bool contains(const wchar_t& subStr) { return (this->find(subStr) != std::wstring::npos); }

        inline WString without_startI(wstring str, int i) { return subwstring(indexOf(str, i) + int(str.length()), int(length())); }
        inline WString without_startF(wstring str) { return subwstring(firstOf(str) + int(str.length()), int(length())); }
        inline WString without_start(wstring str) { return subwstring(indexOf(str) + int(str.length()), int(length())); }
        inline WString without_startL(wstring str) { return subwstring(lastOf(str) + int(str.length()), int(length())); }
        inline WString without_endI(wstring str, int i) { return subwstring(0, indexOf(str, i)); }
        inline WString without_end(int length_) { return subwstring(0, int(length()) - length_ - 1); }
        inline WString without_start(int length_) { return subwstring(length_, int(length())); }
        inline WString without_endF(wstring str) { return subwstring(0, firstOf(str)); }
        inline WString without_endL(wstring str) { return subwstring(0, lastOf(str)); }
        inline WString without_end(wstring str) { return subwstring(0, indexOf(str)); }
        inline WString without_start() { return without_start(1); }
        inline WString without_end() { return without_end(1); }

        inline WString last(int length_) { return subwstring(int(length()) - length_, int(length())); }
        inline WString first(int length_) { return subwstring(0, length_); }
        inline WString first() { return first(1); }
        inline WString last() { return last(1); }


        template<std::size_t times=1>
        WString replace(const std::wstring& from, const std::wstring& to) {
            if constexpr (times == 1) {
                wstring ret = *this;
                std::size_t pos = 0;
                pos = ret.find(from, pos);
                if (pos != std::wstring::npos) ret.replace(pos, from.length(), to);
                return ret;
            } else {
                wstring ret = *this;
                std::size_t pos = 0;
                for (std::size_t i=0;(pos = ret.find(from, pos)) != std::wstring::npos && i < times;i++) {
                    ret.replace(pos, from.length(), to);
                    pos += to.length();
                }
                return ret;
            }
        }

        WString replace_all(const std::wstring& from, const std::wstring& to);

        WString delete_(const std::wstring& from) { return replace(from, L""); }

        WString delete_all(const std::wstring& from) { return this->replace_all(from, L""); }

        inline bool cmp(const basic_string& str) { return this->compare(str); }

        Arrays::Array<WString> split(wstring spliter) {
            Arrays::Array<WString> ret;
            std::size_t pos = 0;
            while ((pos = this->find(spliter)) != WString::npos) {
                WString token = this->substr(0, pos);
                if (!token.empty())
                    ret.push_back(token);
                this->erase(0, pos + 1);
            }
            if (!this->empty())
                ret.push_back(L"");
            return ret;
        }
        Arrays::Array<WString> split(wchar_t spliter) {
            Arrays::Array<WString> ret;
            std::size_t pos = 0;
            while ((pos = this->find(spliter)) != WString::npos) {
                WString token = this->substr(0, pos);
                if (!token.empty())
                    ret.push_back(token);
                this->erase(0, pos + 1);
            }
            if (!this->empty())
                ret.push_back(L"");
            return ret;
        }
        Arrays::Array<WString> splitTo(int number);

        inline bool containsAll(std::vector<std::wstring> subStr);
        inline bool containsAll(const std::wstring* subStr);
        inline bool containsAll(std::vector<wchar_t> subStr);
        inline bool containsAll(const wchar_t* subStr);

        inline bool containsPieces(const std::wstring subStr) { return this->containsPieces(subStr.c_str()); }
        inline bool containsPieces(std::vector<std::wstring> subStr);
        inline bool containsPieces(const std::wstring* subStr);

        inline bool containsOneOf(std::vector<std::wstring> subStr);
        inline bool containsOneOf(const std::wstring* subStr);
        inline bool containsOneOf(std::vector<wchar_t> subStr);
        inline bool containsOneOf(const wchar_t* subStr);

        std::size_t indexOf(wstring index, std::size_t number);
        std::size_t indexOf(char index, std::size_t number);
        std::size_t numberOf(wstring splinter);
        std::size_t numberOf(char splinter);
        std::size_t firstOf(wstring index);
        std::size_t indexOf(wstring index);
        std::size_t lastOf(wstring index);
        std::size_t firstOf(char index);
        std::size_t indexOf(char index);
        std::size_t lastOf(char index);

        WString trim();

        WString pad_right(std::size_t total_width, char padding_char = ' ');
        WString pad_left(std::size_t total_width, char padding_char = ' ');
        WString reverse();

        inline WString operator ()(int start, int end) { return subwstring(start, end); }
        inline wchar_t operator [] (int number) { return this->c_str()[number]; }

        inline WString operator - (const WString& str) { return WString(replace_all(str, L"")); }
        inline WString operator - (const wstring& str) { return WString(replace_all(str, L"")); }
        inline void operator -= (const WString& str) { this->assign(replace_all(str, L"")); }
        inline void operator -= (const wstring& str) { this->assign(replace_all(str, L"")); }

        inline WString operator / (std::size_t num[2]) { return subwstring(num[0], num[1]); }
        inline WString operator / (wstring num[2]) { return subwstring(num[0], num[1]); }
        inline Arrays::Array<WString> operator / (wstring str) { return split(str); }

        inline unsigned int touint() { return TXT_TO_NUM<unsigned int>(*this); }
        inline float tofloat() { return TXT_TO_NUM<float>(*this); }
        inline int toint() { return TXT_TO_NUM<int>(*this); }

		template<typename num_t> requires std::is_arithmetic_v<num_t>
        num_t tonum() { return TXT_TO_NUM<num_t>(*this); }

        // operators
        inline WString operator * (std::size_t indexs) {
            WString ret = *this;
            for (std::size_t i = 0; i < indexs; i++) ret += ret;
            return ret;
        }
        inline void operator *=(std::size_t indexs)
            { for (std::size_t i = 0; i < indexs; i++) this->operator+=(*this); }
    };
}

export using wstr_t = str::WString;
