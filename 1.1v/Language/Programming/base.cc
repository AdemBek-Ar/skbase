export module skbase.lang.base;

import std;

export namespace Language {
	using token_t_t = unsigned int;
	bool isoprtr(const char& c)  {
		if (c == '+' or c == '-' or c == '*' or c == '/' or c == '^' or c == ',' or
			c == '!' or c == ':' or c == '&' or c == '|' or c == '%' or c == '.' or
			c == '~' or c == '?' or c == '<' or c == '>' or c == '=' or
			c == '[' or c == ']' or c == '{' or c == '}' or c == '(' or
			c == ')' or c == '$' or c == '#' or c == '@' or c == ';'
			) return true;
		return false;
	}
}
