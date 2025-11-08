export module skbase.lang.token;

import skbase.memory.array;
import skbase.str.String;
import skbase.lang.base;
import skbase.files;
import skbase.base;
import std;

#include "../../defines.h++"

export namespace Language {
	class Keywords : public Arrays::Array<str_t> {
	public :
		using Arrays::Array<str_t>::Array;

		Keywords(std::string fpath = "res/keywords.ucc")
			:Arrays::Array<str_t>::Array(std::vector<str_t>(str_t(file::get_contents(fpath.c_str())).split('\n'))) {
			if (this->empty()) {
				outn("\e[1;31mthere is no file or the file is empty\e[0m")
				return;
			}
		}

		inline str_t operator[] (token_t_t i) { return this->at(i); }
		inline token_t_t operator[] (str::string keyword)
			{ return this->indexOf(keyword); }
		inline bool is_in(token_t_t type, Arrays::Array<str_t> kws) {
			for (str_t kw : kws)
				if (type == this->operator[](kw))
					return true;
			return false;
		}
		inline bool is_in(token_t_t type) {
			for (str_t kw : *static_cast<Arrays::Array<str_t>*>(this))
				if (type == this->operator[](kw))
					return true;
			return false;
		}

		auto operator() (str_t str, str_t value="");
	};
	using kwm_t = Keywords;



	class Token {
	public :
		token_t_t type : 8;
		std::optional<str_t> value;
		std::size_t pos=0, length=0, line=0;

		Token(token_t_t type, str_t value) :type(type), value(value) {}
		Token(token_t_t type = 0) :type(type) {}

		inline str_t tostring(kwm_t& m)
			{ return m[this->type]; }
		inline str_t pos_info() {
			str_t err_rang = (this->length == 1)? std::to_string(this->pos) : std::to_string(this->pos)+"<->"+std::to_string(this->length);
			return "{"+std::to_string(this->line)+": "+err_rang+"}";
		}

		inline void print(kwm_t& m) {
			str::String txt = tostring(m);
			if (!txt.cmp("\\n")) std::cout << '\n';
			else if (!txt.cmp("\\t")) std::cout << '\t';
			else {
				if (value.has_value()) txt += ", " + value.value();
				std::cout << "\e[1;36m{\e[0m " << txt << " \e[1;36m}\e[0m ";
			}
		}

		bool operator==(Token other ) const { return this->type != other.type; }
		bool operator==(token_t_t other) const { return this->type != other  ; }
		bool operator!=(Token other ) const { return this->type == other.type; }
		bool operator!=(token_t_t other) const { return this->type == other  ; }

		inline bool isin_type_range(const char* ftype, kwm_t& m, const char* ltype)
			{ return type > m.indexOf(ftype) and type < m.indexOf(ltype); }
		inline bool is_type(kwm_t& m)
			{ return this->isin_type_range("short", m, "non_t"); }

		inline token_t_t mtt(token_t_t i=1) { return this->type+i; }
		inline bool is_kw(kwm_t& m) {
			str::String txt = this->tostring(m);
			return txt[0] == '/' and m.contains(txt);
		}
		inline bool is_sb(kwm_t& m)
			{ return m.is_in(this->type, {"(","[","{"}); }
	};
	using token_t = Token;
}



auto Language::Keywords::operator() (str_t str, str_t value) {
	if (this->contains(str)) {
		if (value.empty())
			if (!str.cmp("#opr")     or !str.cmp("#ident")     or !str.cmp("#typ") or
				!str.cmp("#int_lit") or !str.cmp("#float_lit") or
				!str.cmp("#str_lit") or !str.cmp("#byte_lit")
			)	return  token_t(this->indexOf(str), "");
			else return token_t(this->indexOf(str));
		else return token_t(this->indexOf(str), value);
	}
	if (isoprtr(str.at(0)))
		return token_t(this->operator[]("#opr"), str);
	if (str.length() > 1)
		if (!str.last(2).cmp("_t") or !str.last(2).cmp("_T"))
			return token_t(this->operator[]("#typ"), str);
	return token_t(this->operator[]("#ident"), str);
}
