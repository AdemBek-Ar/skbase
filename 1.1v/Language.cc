export module __sk_lib_Language__;

import __sk_lib_base__;
import __sk_lib_String__;
import __sk_lib_Arrays__;
import __sk_lib_Files__;
import std;

#include "defines.h++"

#define msg_Warning str_t("\e[1;33mWarning\e[0m")
#define msg_Error   str_t("\e[1;31mError\e[0m")
#define msg_Node   str_t("\e[1;36mNode\e[0m")

export namespace Language {
	using token_t_t = unsigned int;

	bool isoprtr(const char& c);

	class Keywords : public Arrays::Array<str_t> {
	public :
		inline Keywords(std::string fpath = "res/keywords.ucc")
			: Arrays::Array<str_t>(str_t(Files::get_file_contents(fpath.c_str())).split('\n')) {
			if (this->empty()) {
				outn("\e[1;31mthere is no file or the file is empty\e[0m")
				return;
			}
		}

		inline str_t operator[] (token_t_t i) { return this->at(i); }
		inline token_t_t operator[] (str::string keyword)
			{ return this->indexOf(keyword); }
		inline bool is_in(token_t_t type, Arrays::Array<str_t> kws) {
			for (str_t kw : kws) if (type == this->operator[](kw)) return true;
			return false;
		}

		inline auto operator() (str_t str, str_t value="");

		static inline Keywords ucean_kws() { return Keywords("res/keywords.ucc"); }
	};
	using kwm_t = Keywords;



	class Token {
	public :
		token_t_t type;
		std::optional<str_t> value;
		std::size_t pos=0, length=0, line=0;

		Token(token_t_t type, str_t value) :type(type), value(value) {}
		Token(token_t_t type = 0) :type(type) {}

		inline str_t tostring(kwm_t m) const
			{ return m[this->type]; }
		inline str_t pos_info() {
			str_t err_rang = (this->length == 1)? std::to_string(this->pos) : std::to_string(this->pos)+"<->"+std::to_string(this->length);
			return "{"+std::to_string(this->line)+": "+err_rang+"}";
		}
		void print(kwm_t m);

		bool operator==(Token other ) const;
		bool operator==(token_t_t other) const;
		bool operator!=(Token other ) const;
		bool operator!=(token_t_t other) const;

		inline bool isin_type_range(const char* ftype, kwm_t m, const char* ltype)
			{ return type > m.indexOf(ftype) and type < m.indexOf(ltype); }
		inline bool is_type(kwm_t m)
			{ return this->isin_type_range("short", m, "non_t"); }

		inline token_t_t mtt(token_t_t i=1) { return this->type+i; }
		inline bool is_kw(kwm_t m);
		inline bool is_sb(kwm_t m);
	};
	using token_t   = Token;



	inline auto Keywords::operator() (str_t str, str_t value) {
		if (this->contains(str)) {
			if (value.empty()) return token_t(this->indexOf(str));
			else return token_t(this->indexOf(str), value);
		}
		if (isoprtr(str.at(0)))
			return token_t(this->operator[]("#opr"), str);
		return token_t(this->operator[]("#ident"), str);
	}



	namespace Node {
		class Node {
		public :
			std::optional<token_t> token;
			Arrays::Array<Node*> childs;

			inline Node(token_t token, Arrays::Array<Node*> childs={})
				:childs(childs) {
				this->token = token;
			}
			virtual str_t tostring(kwm_t m) {
				str_t ret;
				if (this->token.has_value()) {
					if (this->token.value().value.has_value())
						ret = this->token.value().value.value();
					else ret = this->token.value().tostring(m);
				}
				for (Node* n : this->childs) if (n->token.has_value()) {
					if (n->token.value().value.has_value())
						ret = n->token.value().value.value();
					else ret = n->token.value().tostring(m);
				}
				if (this->token.value().type != m["#str_lit"] && this->token.value().type != m["#char_lit"])
					ret=
					ret .replace_all("\\n", "\n" ).replace_all("\\t", "\t" ).replace_all("\\f", "\f" )
						.replace_all("\\r", "\r" ).replace_all("\\a", "\a" ).replace_all("\\b", "\b" )
						.replace_all("\\\\", "\\").replace_all("\\\"", "\"").replace_all("\\\'", "\'");
				return ret;
			}
		};
		using node_t = Node;


		class Return : public Node {
		public :
			Return(Arrays::Array<node_t*> returned={})
				:node_t(token_t(10), returned) {}

			str_t tostring(kwm_t m) override {
				str_t ret = "return ";
				for (node_t* n : this->childs) ret += n->tostring(m);
				return ret+';';
			}
		};


		class MainFunction : public Node {
		public :
			MainFunction(Arrays::Array<node_t*> block={})
				:node_t(token_t(),block) {}

			str_t tostring(kwm_t m) override {
				str_t ret = "int main(";
				std::size_t i=0;
				if (this->childs[i]->token.has_value())
					if (this->childs[i]->token.value().type == m["$"]) {
						ret += "int argc, char **argv";
						i++;
					}
				ret += ") {";
				for (std::size_t size= this->childs.size(); i<size; i++) ret += this->childs[i]->tostring(m);
				return ret+"\n\treturn 0;\n}";
			}
		};


		class DeclVariable : public Node {
		public :
			token_t name;
			DeclVariable(token_t type, token_t name, Arrays::Array<node_t*> value={})
				:node_t(type,value), name(name) {}

			str_t tostring(kwm_t m) override {
				if (!this->name.value.has_value()) {
					outn(msg_Warning+": there is no name for this variable in line; " << this->token->pos_info())
					static std::size_t num= 0;
					str_t oname= "non_named_variable"+std::to_string(num++);
					outn("\tdo you want to give is a name if no it will named \e[1;32m'"<<oname<<"'\e[0m [Y/*]?")
						char ansr;
						std::cin >> ansr;
						if (ansr == 'y' or ansr == 'Y')
							std::getline(std::cin, oname);
					name= token_t(m["#ident"], oname);
				}
				str_t ret;
				if (this->token.has_value()) {
					str_t type= (this->token->value.has_value())? this->token->value.value() : this->token->tostring(m);
					ret += type + " " + name.value.value();
				}
				for (node_t* n : this->childs)
					ret += n->tostring(m);
				return ret+';';
			}
		};



		class DeclClass : public Node {
		public :
			Arrays::Array<token_t> extentions;
			DeclClass(
				token_t name, Arrays::Array<node_t*> block={},
				Arrays::Array<token_t> extentions={}
			) :node_t(name,block), extentions(extentions) {}

			str_t tostring(kwm_t m) override {
				if (!this->token.has_value() or !this->token->value.has_value()) {
					outn(msg_Warning+": there is no name for this class in line; " << this->token->pos_info())
					static std::size_t num= 0;
					str_t oname= "non_named_variable"+std::to_string(num++);
					outn("\tdo you want to give is a name if no it will named \e[1;32m'"<<oname<<"'\e[0m [Y/*]?")
						char ansr;
						std::cin >> ansr;
						if (ansr == 'y' or ansr == 'Y')
							std::getline(std::cin, oname);
					this->token= token_t(m["#ident"], oname);
				}
				str_t ret= "class "+this->token->value.value();
				for (auto extnd : extentions) {
					if (static_cast<int>(extnd.type) > 40 and static_cast<int>(extnd.type) < 44) {
						if	  (extnd.type == m["/pbl" ]   ) ret += " public";
						else if (extnd.type == m["/prv" ]  )  ret += " private";
						else if (extnd.type == m["/prtc"])	ret += " protected";
					} else if (extnd.value.has_value())
						ret += " "+extnd.value.value();
				}
				ret += " {";
				for (node_t* n : this->childs)
					ret += n->tostring(m);
				return ret+"};";
			}
		};



		class DeclFunction : public Node {
		public :
			Arrays::Array<node_t*> args;
			DeclFunction(
				token_t name, Arrays::Array<node_t*> block={},
				Arrays::Array<node_t*> args={}
			) :node_t(name,block), args(args) {}

			str_t tostring(kwm_t m) override {
				if (!this->token.has_value() or !this->token->value.has_value()) {
					outn(msg_Warning+": there is no name for this function in line; " << this->token->pos_info())
					static std::size_t num= 0;
					str_t oname= "non_named_variable"+std::to_string(num++);
					outn("\tdo you want to give is a name if no it will named \e[1;32m'"<<oname<<"'\e[0m [Y/*]?")
						char ansr;
						std::cin >> ansr;
						if (ansr == 'y' or ansr == 'Y')
							std::getline(std::cin, oname);
					this->token= token_t(m["#ident"], oname);
				}
				str_t ret= "class "+this->token->value.value() + " {";
				for (auto arg: args) if (arg->token.has_value()) {
					if (static_cast<int>(arg->token->type) > 40 and static_cast<int>(arg->token->type) < 44) {
						if	  (arg->token->type == m["/pbl" ]   ) ret += " public:";
						else if (arg->token->type == m["/prv" ]  )  ret += " private:";
						else if (arg->token->type == m["/prtc"])	ret += " protected:";
					} else if (!m.is_in(arg->token->type, {"(", ")", ","}))
						ret += "\n\t"+arg->tostring(m);
				}
				ret += "\n\tinline auto operator() () {";
				for (node_t* n : this->childs)
					ret += n->tostring(m);
				return ret+"\n\t}\n};";
			}
		};



		class CallFunction : public Node {
		public :
			CallFunction(token_t name, Arrays::Array<node_t*> block={}) 
				:node_t(name,block) {}

			str_t tostring(kwm_t m) override {
				str_t ret= this->token->value.value() + "{";
				for (node_t* n : this->childs)
					ret += n->tostring(m);
				return ret+"}();";
			}
		};
	}

#define using_node_short_names\
	using decvnode_t = Node::DeclVariable;\
	using epnode_t   = Node::MainFunction;\
	using decfnode_t = Node::DeclFunction;\
	using calfnode_t = Node::CallFunction;\
	using deccnode_t = Node::DeclClass   ;\
	using retnode_t  = Node::Return	  ;\
	using node_t	 = Node::Node		;



	template<bool AUTODBG=false> class Compiler {
	protected:
		std::size_t m_i=0, lines_num= 0;
		str_t m_data, word;
	private:
		std::string filename;
	public :
		kwm_t ucean_kw;
		using_node_short_names

		Compiler(std::string file) : filename(file), tokenaizer(this), ast(this), parser(this) {
			this->m_data = str_t(Files::get_file_contents(file.c_str()))
				.replace_all("\n	", "\n\t").replace_all("\t	", "\t\t");
			this->ucean_kw = kwm_t::ucean_kws();
		}

		inline void setup_token_for_error(token_t tk, bool clear_word= true) {
			if (clear_word) {
				this->word.clear();
				for (std::size_t i=0; i<tk.length; i++) this->word += ' ';
			}
			this->m_i = tk.pos+tk.length;
			this->lines_num = tk.line;
		}
		inline void print_error_location(std::string err_msg, std::string color="\e[1;31m") {
			static Arrays::Array<str_t> src_lines;
				for (char c_ : this->m_data) {
					static str_t text;
					if (c_ == '\n') {
						if (text.empty()) text += ' ';
						src_lines.add(text);
						text.clear();
					} else text += c_;
				}

			std::size_t err_char_num = 0;
				for (;this->m_i-err_char_num>0 and this->m_data[this->m_i-err_char_num] != '\n';) err_char_num++;
				err_char_num -= this->word.size();
			if (static_cast<long int>(err_char_num) < 0) err_char_num= 0;
			std::cerr << err_msg << '\n';
			std::string err_range = (this->word.size()<=1)? "" : std::string("<->") + std::to_string(err_char_num+this->word.size());
			std::cerr << "in line {" << this->lines_num+1 << ": " << err_char_num << err_range <<"}\n";
			std::cerr << "|--" << this->lines_num+1 << ":\t\e[1;35m\"\e[0m";
				for (std::size_t i=0, size=src_lines[this->lines_num].size(); i<size; i++)
					if (i >= err_char_num and i <= (err_char_num+((this->word.size()<=1)?0:this->word.size())))
						std::cerr << color << src_lines[this->lines_num][i] << "\e[0m";
					else if (src_lines[this->lines_num][i] != '\t') std::cerr << src_lines[this->lines_num][i];

			std::cerr << "\e[1;35m\"\e[0m\n    \t "; {
				for (std::size_t i=0, size=src_lines[this->lines_num].size(); i<size; i++)
					if (i >= err_char_num and i <= (err_char_num+((this->word.size()<=1)?0:this->word.size())))
						std::cerr << color << "^" << "\e[0m";
					else if (src_lines[this->lines_num][i] != '\t') std::cerr << ' ';
			} std::cerr << '\n';
		}

		inline Arrays::Array<token_t> getks_ov(Arrays::Array<str_t> types={}) {
			Arrays::Array<token_t> ret;
			for (str_t type: types)
				ret.add(token_t(this->ucean_kw[type])); 
			return ret;
		}
		inline token_t getk_ov(str_t type) {
			token_t tk;
			if (this->ucean_kw.contains(type)) tk= this->ucean_kw[type];
			else tk= this->ucean_kw(type);
			tk.length = type.length();
			tk.pos = this->m_i-tk.length;
			tk.line = this->lines_num;
			return tk;
		}
		inline token_t getk(str_t type="") {
			token_t tk;
			if (!this->word.empty()) {
				if (type.empty())
					tk= this->ucean_kw(this->word);
				else
					tk= this->ucean_kw(type, this->word);
				tk.length = this->word.length();
				this->word.clear();
			}
			tk.pos = this->m_i-tk.length;
			tk.line = this->lines_num;
			return tk;
		}

		class Tokenaizer {
		private: Compiler *cmplr;
		public :
			Tokenaizer(Compiler *cmplr) :cmplr(cmplr) {}



			Arrays::Array<token_t> tokenaze() {
				Arrays::Array<token_t> ret; // returned value
				// mainloop
				for (std::size_t size=cmplr->m_data.length(); is_peeked(cmplr->m_i < size) ; cmplr->m_i++) {
					static char c				 = inpos(); // save help_char as static *for make no memory gaps
					c							 = inpos(); // set help_char value for each time

					// get a STR_LIT
					if (bool is_instring_block= (c == '\"' or c == '`') and cmplr->m_data[(cmplr->m_i-1)%size] != '\\') {
						cmplr->m_i++;
						  while ( is_peeked(is_instring_block) ) {
							if (inpos() == c and cmplr->m_data[cmplr->m_i-1] != '\\') {
								is_instring_block = !is_instring_block;
								cmplr->m_i++;
								continue;
							}
							addchar(next());
						  }
						cmplr->m_i--;

						if (c == '`') cmplr->word = "R\"("+cmplr->word+")\"";
						else cmplr->word = "\""+cmplr->word+"\"";
						ret.add(cmplr->getk("#str_lit"));
					}
					// get what is not STR_LIT
					else {
						if (c == '\n') {
							add_newline:
							if (cmplr->m_data[(cmplr->m_i-1)%size] != '\n')
								ret.add(cmplr->getk_ov("\\n"));
							cmplr->lines_num++;
							continue;
						}
						if (c == ' ') continue;
						else if (c == '\t') {
							ret.add(cmplr->getk_ov("\\t"));
							continue;
						}
						// get a CHAR_LIT
						else if (c == '\'') {
							cmplr->m_i++;
							if (inpos() == '\\') addchar(next());
							addchar(next());
							if (inpos() != '\'') {
								cmplr->print_error_location(msg_Warning+": [syntax error]; the char_litral is longer than is have tobe ---> you add a lot of characters", "\e[1;33m");
								std::cout << "\t\e[1;35mthe compiler may fix that automaticly\e[0m\n";
								str_t oldchar = cmplr->word;
								cmplr->word.clear();
								for (;inpos() != '\''; cmplr->m_i++) if (inpos() == '\n' or inpos() == ' ' or isoprtr(inpos())) {
									if (isoprtr(inpos())) {
										cmplr->word = cmplr->word.without_end(1);
										cmplr->m_i--;
									}
									cmplr->m_i--;
									cmplr->print_error_location(msg_Warning+": [syntax error]; missing \e[1;35m\"'\"\e[0m ---> you don't close the char_litral block", "\e[1;33m");
									std::cout << "\t\e[1;35mthe compiler may fix that automaticly\e[0m\n";
									cmplr->m_i++;
									break;
								} else addchar(inpos());
								cmplr->word = '\''+oldchar+'\'';
							}

							ret.add(cmplr->getk("#char_lit"));
							if (inpos() == '\n' and cmplr->m_data[(cmplr->m_i-1)%size] != '\n') {
								cmplr->lines_num++;
								ret.add(cmplr->getk_ov("\\n"));
							}
							continue;
						}
						else {
							addchar(c);
							// get an IDENTIFIER or TYPEs
							if (std::isalpha(c) or c == '_') {
								cmplr->m_i++;
								add_ident:
									  while ( is_peeked(std::isalnum(inpos()) or inpos() == '_') )
										  addchar(next());
									cmplr->m_i--;
									if (c == '/' or c == '#')
										if (!cmplr->ucean_kw.contains(cmplr->word))
											unexisted_kw_err(ret);
									ret.add(cmplr->getk());
							}
							// get a NUMBER (INT | FLOAT)_LIT
							else if (std::isdigit(c)) {
								cmplr->m_i++;
								static bool is_float = false;
								while ( is_peeked(std::isdigit(inpos()) or inpos() == '.') ) {
									if (inpos() == '.' and is_float) {
										if (AUTODBG) {
											cmplr->print_error_location(msg_Warning+": [syntax error]; doublycated float litral point", "\e[1;33m");
											std::cout << "\t\e[1;36mthe compiler will fix it automaticly\e[0m\n";
										} else cmplr->print_error_location(msg_Error+": [syntax error]; doublycated float litral point");
										next();
									} else {
										if (inpos() == '.' and !is_float) is_float = true;
										addchar(next());
									}
								}
								cmplr->m_i--;

								if (is_float) {
									ret.add(cmplr->getk("#float_lit"));
									is_float = false;
								}
								else	
									ret.add(cmplr->getk("#int_lit"));
							}
							// get a KEY_WORDs that are All Start With '/'
							else if (c == '/') {
								cmplr->m_i++;
								if (std::isalnum(inpos()) or inpos() == '_')
									goto add_ident;
								else if (inpos() == '-') {
									addchar(next());
									if (inpos() == '>') addchar(next());
									else {
										if constexpr (AUTODBG) {
											cmplr->print_error_location(msg_Warning+": faurget the '>' after '/-' define of program EntryPoint", "\e[1;33m");
											addchar('>');
											continue;
										}
										else cmplr->print_error_location(msg_Error+": faurget the '>' after '/-' define of program EntryPoint");
									}
								}
								else if (inpos() == '/') {
									for (;is_peeked(next() != '\n');)
									  if (inpos() == '\n') {
										cmplr->word.clear();
										goto add_newline;
									  }
								}
								else if (inpos() == '*') {
									cmplr->m_i++;
									for (;;) if (is_peeked(next() == '*' and next() == '/')) {
										cmplr->word.clear();
										break;
									} else if (inpos() == '\n') cmplr->lines_num++;
									cmplr->m_i--;
									continue;
								}
								cmplr->m_i--;
								if (cmplr->ucean_kw.contains(cmplr->word)) {
									token_t tk_= cmplr->getk();
									if (tk_.type == cmplr->ucean_kw["/->"]) {
										static token_t main_function_first_token;
										if (main_function_first_token.type == cmplr->ucean_kw["/->"]) {
											if constexpr (AUTODBG) {
												cmplr->print_error_location(msg_Warning+": the programe 'EntryPoint' is already declared", "\e[1;33m");
												cmplr->setup_token_for_error(main_function_first_token);
												cmplr->print_error_location(msg_Node+": declare in", "\e[1;36m");
												cmplr->setup_token_for_error(tk_); {
													auto tks= cmplr->getks_ov({"/fn", "#ident", "(", ")"});
													tks[1].value= "main";
													ret.add(tks);
												}
												std::cout << "\t\e[1;36mthe compiler will add \e[1;32m'/fn main()'\e[1;36m instead of it\e[0m\n";}
											else {
												cmplr->print_error_location(msg_Error+": the programe 'EntryPoint' is already declared");
												cmplr->setup_token_for_error(main_function_first_token);
												cmplr->print_error_location(msg_Node+": declare in", "\e[1;36m");
												cmplr->setup_token_for_error(tk_);
											}
										} else ret.add(main_function_first_token=tk_);
									} else ret.add(tk_);
								} else unexisted_kw_err(ret);
							}
							// get a KEY_WORDs that are All Start With '#' or Defines
							else if (c == '#') {
								cmplr->m_i++;
								if (std::isalnum(inpos()) or inpos() == '_')
								  while ( is_peeked(std::isalnum(inpos()) or inpos() == '_') )
									addchar(next());
								else if (inpos() == '-') {
									addchar(next());
									if (inpos() == '>') addchar(next());
									else cmplr->print_error_location(msg_Error+": faurget the '>' after '#-' to add a this block to the start of programe EntryPoint");
								}
								cmplr->m_i--;

								if (cmplr->ucean_kw.contains(cmplr->word)) ret.add(cmplr->getk());
								else unexisted_kw_err(ret);
							}
							// get a OPERATIONS
							else if (isoprtr(c)) {
								cmplr->m_i++;
								  while ( is_peeked(isoprtr(inpos())) )
									addchar(next());
								cmplr->m_i--;

								if (cmplr->ucean_kw.contains(cmplr->word)) ret.add(cmplr->getk());
								else if (!cmplr->word.cmp(")(")) ret.add(cmplr->getks_ov({")", "("}));
								else if (!cmplr->word.cmp("&&")) ret.add(cmplr->getk_ov("and"));
								else if (!cmplr->word.cmp("||")) ret.add(cmplr->getk_ov("or"));
								else ret.add(cmplr->getk());
							}
						} if (!cmplr->word.empty()) cmplr->word.clear();
					}
				}
				ret.add(token_t(0));
				return ret;
			}

		protected:
			inline char inpos  (std::size_t i=0) { return cmplr->m_data[cmplr->m_i+i]; }
			inline char previos() { return cmplr->m_data.at(cmplr->m_i--); }
			inline char next   () { return cmplr->m_data.at(cmplr->m_i++); }

			inline void addchar(const char& c) { cmplr->word += c; }

			inline bool is_peeked(bool ret) { return ret and this->peek().has_value(); }

			template<std::size_t ahead=1> [[nodiscard]]
			inline std::optional<char> peek() {
				if (cmplr->m_i + ahead >= cmplr->m_data.length()) return {};
				return cmplr->m_data.at(cmplr->m_i);
			}

			inline void back_inline_to_add(Arrays::Array<token_t> &ret, std::vector<token_t> addeds) {
				Arrays::Array<token_t> shifted_elements;
				int tabs = 0;
				for (std::size_t i=ret.size()-1; i>0; i--) {
					if (ret[i].type == 0) {
						for (int i=0; i<tabs; i++) ret.add(token_t(1));
						ret.add(addeds);
						ret.add(token_t(0));
						for (std::size_t j=shifted_elements.size()-1; j!=0; j--)
							ret.add(shifted_elements[j]);
						ret.add(shifted_elements[0]);
						break;
					} else {
						if (ret[i].type == 1) tabs++;
						shifted_elements.add(--ret);
					}
				}
			}

			inline void unexisted_kw_err(Arrays::Array<token_t> &ret) {
				if constexpr (AUTODBG) {
					cmplr->print_error_location(msg_Warning+": there is no keyword call \e[1;31m'" + cmplr->word + "'\e[0m select one with", "\e[1;33m");
					for (str_t& kw : cmplr->ucean_kw) if (cmplr->word.contains(kw) and kw.cmp("/")) {
						std::cout << "the token \e[1;31m'"<<cmplr->word<<"'\e[0m are containes the keyword \e[1;32m'"<<kw
							<< "'\e[0m do you want to replace it with this keyword [Y/n/R]?\n"
							<< "\e[1;30myou have to answer [Yes/No/Replace With]\e[0m :";
						char c;
						std::cin >> c;
						if (c == 'y' or c == 'Y') {
							cmplr->word.clear();
							cmplr->word += kw;
							ret.add(cmplr->getk());
							break;
						} else if (c == 'y' or c == 'Y') {
							outn("\e[1;30mEnter the keyword that you want:\e[0m")
							cmplr->word.clear();
							std::getline(std::cin, cmplr->word);
							ret.add(cmplr->getk());
							break;
						}
					}
				} else
					cmplr->print_error_location(msg_Error+": there is no keyword call \e[1;31m'" + cmplr->word + "'\e[0m plase fix that");
			}

			inline std::size_t get_tab_level(Arrays::Array<token_t> &ret) {
				std::size_t ret_;
				for (std::size_t i=ret.size()-1; i>0; i--)
					if	  (ret[i].type == 0) break;
					else if (ret[i].type == 1) ret_++;
				return ret_;
			}
		} tokenaizer;



		class AbastractSyntaxTree		 ;
		class Parser					  ;
		using AST = AbastractSyntaxTree;



		class AbastractSyntaxTree {
		public :
			using_node_short_names
		private: Compiler *cmplr;
			Arrays::Array<node_t> m_nodes;
		public :
			AbastractSyntaxTree(Compiler *cmplr) : cmplr(cmplr) {}
		} ast;



		class Parser {
		public :
			using_node_short_names

			Parser(Compiler *cmplr) : cmplr(cmplr) {}

			inline void set_tokens(Arrays::Array<token_t> tokens) { this->m_tokens = tokens; }
			inline Arrays::Array<token_t> get_tokens() { return this->m_tokens; }

			Arrays::Array<node_t*> parse(std::size_t root_tab_level=0, std::function<bool()> other= []{ return true; }) {
				std::size_t tab_level= root_tab_level;
				Arrays::Array<node_t*> ret;

				for (; is_peeked(this->m_ti<m_tokens.size() and tab_level>=root_tab_level and other()); this->m_ti++) {
					token_t tk = inpos();
					if (tk.type == cmplr->ucean_kw["#ident"] and tk.value.has_value()) {
						ret.add(new node_t(inpos()));
					}
					else {
						// new-line
						if (inpos().type == 0) {
							ret.add(new node_t(next()));
							tab_level=0;
							tk = inpos();
							if (is_peeked(tk.type != 1)) {
								m_ti--;
								continue;
							}
						}
						//tab
						if (tk.type == 1) {
							if (tab_level == root_tab_level) tab_level= 0;
							while(is_peeked(next().type == 1)) {
								ret.add(new node_t(tk));
								tab_level++;
							} this->m_ti-=2;
						}
						//declare var
						else if (tk.is_type(cmplr->ucean_kw)) {
							this->m_ti++;
							if (is_peeked(inpos().type == cmplr->ucean_kw["#ident"])) {
								Arrays::Array<node_t*> value;
								token_t name = next();
								if (is_peeked(inpos().type == cmplr->ucean_kw["="])) {
									token_t eql= next();
									value.add(new node_t(eql));
									value.add(parse(
										root_tab_level,
										[&] { return is_peeked(inpos().type != 0); }
									));
								}
								decvnode_t* var= new decvnode_t(tk, name, value);
								ret.add(var);
							} else ret.add(new node_t(tk));
							this->m_ti--;
						}
						//return
						else if (tk.type== cmplr->ucean_kw["/ret"]) {
							this->m_ti++;
							retnode_t* var= new retnode_t(parse(
								root_tab_level,
								[&] { return is_peeked(inpos().type != 0); }
							));
							ret.add(var);
						}
						// main function
						else if (tk.type == cmplr->ucean_kw["/->"]) {
							this->m_ti++;
							epnode_t* main_fun= new epnode_t(parse(1));
							ret.add(main_fun);
						}
						// what ever
						else ret.add(new node_t(inpos()));
					}
				} return ret;
			}

		private:
			Arrays::Array<token_t> m_tokens;
			std::size_t tabs_level=0, m_ti=0;
			Compiler *cmplr;

			inline void print_error_location(token_t tk, std::string err_msg) {
				cmplr->setup_token_for_error(tk);
				cmplr->print_error_location(err_msg);
			}

			inline bool is_peeked(bool ret=true) { return this->peek().has_value() and ret; }

			template<std::size_t ahead=1> [[nodiscard]]
			inline std::optional<token_t> peek() {
				if (m_ti + ahead >= m_tokens.size()) return {};
				return m_tokens.at(m_ti);
			}

			inline token_t inpos  (std::size_t i=0) { return this->m_tokens[this->m_ti+i]; }
			inline token_t previos(			   ) { return this->m_tokens[this->m_ti--]; }
			inline token_t next   (			   ) { return this->m_tokens[this->m_ti++]; }
		} parser;



		inline void compile() {
			auto tokens = this->tokenaizer.tokenaze();
			parser.set_tokens(tokens);
			outn("tokens start")
			if (!tokens.empty())
			  for (auto token : parser.get_tokens())
				token.print(ucean_kw);
			else {
				outn("\tno code")
				return;
			}
			outn("\n\ntokens end")
			std::cout << '\n';

			outn("\nparse-code start")
			auto parses = parser.parse();
			outn("\nparse-code end")
			outn("\n\ngen-code")
			if (!parses.empty())
			  for (auto& parse : parses)
				outn(parse->tostring(ucean_kw))
			else {
				outn("\tno code")
				return;
			}
		}
	};
}
