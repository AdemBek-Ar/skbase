export module skbase.lang.lexer;

import skbase.base;
import skbase.str.String;
import skbase.memory.array;
import skbase.files;
import std;

import skbase.lang.token;
import skbase.lang.base;

#include "../../defines.h++"

export namespace Language {
	template<bool AUTODBG=false> class Lexer {
	private:
		std::string filename;
	public :
		kwm_t ucean_kw;

		Lexer(std::string file, str_t path= "res/keywords.ucc") : filename(file) {
			this->m_data = str_t(file::get_contents(file.c_str()))
				.replace_all("\n	", "\n\t").replace_all("\t	", "\t\t");
			this->ucean_kw(path);
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
			else tk= (token_t)this->ucean_kw(type);
			tk.length = type.length();
			tk.pos = this->m_i-tk.length;
			tk.line = this->lines_num;
			return tk;
		}



		inline token_t getk(str_t type="") {
			token_t tk;
			if (!this->word.empty()) {
				if (type.empty())
					tk= (token_t)this->ucean_kw(this->word);
				else
					tk= (token_t)this->ucean_kw(type, this->word);
				tk.length = this->word.length();
				this->word.clear();
			}
			tk.pos = this->m_i-tk.length;
			tk.line = this->lines_num;
			return tk;
		}



		Arrays::Array<token_t> tks() {
			Arrays::Array<token_t> ret; // returned value
			// mainloop
			for (std::size_t size=this->m_data.length(); is_peeked(this->m_i < size) ; this->m_i++) {
				static char c				 = inpos(); // save help_char as static *for make no memory gaps
				c							 = inpos(); // set help_char value for each time

				if (c == ' ' or c == '\r') continue;

				// get a STR_LIT
				if (bool is_instring_block= (c == '\"' or c == '`') and this->m_data[(this->m_i-1)%size] != '\\') {
					this->m_i++;
					  while ( is_peeked(is_instring_block) ) {
						if (inpos() == c and this->m_data[this->m_i-1] != '\\') {
							is_instring_block = !is_instring_block;
							this->m_i++;
							continue;
						}
						addchar(next());
					  }
					this->m_i--;

					if (c == '`') this->word = "R\"("+this->word+")\"";
					else this->word = "\""+this->word+"\"";
					ret.add(this->getk("#str_lit"));
				}



				// get what is not STR_LIT
				else {
					if (c == '\n') {
						add_newline:
						if (this->m_data[(this->m_i-1)%size] != '\n')
							ret.add(this->getk_ov("\\n"));
						this->lines_num++;
						continue;
					}


					if (c == ' ') continue;
					else if (c == '\t') {
						ret.add(this->getk_ov("\\t"));
						continue;
					}


					// get a CHAR_LIT
					else if (c == '\'') {
						this->m_i++;
						if (inpos() == '\\') addchar(next());
						addchar(next());
						if (inpos() != '\'') {
							this->print_error_location(msg_Warning+": the char_litral is longer than is have to be", "\e[1;33m");
							std::cerr << "\t\e[1;36mthe lexer will fix that automaticly\e[0m\n\n";
							str_t oldchar = this->word;
							this->word.clear();
							for (;inpos() != '\''; this->m_i++) if (inpos() == '\n' or inpos() == ' ' or isoprtr(inpos())) {
								if (isoprtr(inpos())) {
									this->word = this->word.without();
									this->m_i--;
								}
								this->m_i--;
								this->print_error_location(msg_Warning+": missing \e[1;35m\"'\"\e[0m", "\e[1;33m");
								std::cerr << "\t\e[1;36mthe lexer will fix that automaticly\e[0m\n\n";
								this->m_i++;
								break;
							} else addchar(inpos());
							this->word = '\''+oldchar+'\'';
						}

						ret.add(this->getk("#char_lit"));
						if (inpos() == '\n' and this->m_data[(this->m_i-1)%size] != '\n') {
							this->lines_num++;
							ret.add(this->getk_ov("\\n"));
						}
						continue;
					}


					else {
						addchar(c);
						// get an IDENTIFIER or TYPEs
						if (std::isalpha(c) or c == '_') {
							this->m_i++;
							add_ident:
								  while ( is_peeked(std::isalnum(inpos()) or inpos() == '_') )
									  addchar(next());
								this->m_i--;
								if (c == '/' or c == '#')
									if (!this->ucean_kw.contains(this->word))
										if constexpr (AUTODBG) unexisted_kw_err(ret);
										else {
											str_t old_word = this->word;
											this->word.clear();
											this->m_i -= old_word.length()-2;

											this->word += old_word.at(0);
											ret.add(this->getk());
											this->word.clear();

											if (old_word.length() > 1) {
												this->word = old_word.without<true>(std::size_t(1));
												this->m_i += old_word.length()-2;
											}
										}
								ret.add(this->getk());
						}

						// get a NUMBER (INT | FLOAT)_LIT
						else if (std::isdigit(c)) {
							this->m_i++;
							static bool is_float = false;
							while ( is_peeked(std::isdigit(inpos()) or inpos() == '.') ) {
								if (inpos() == '.' and is_float) {
									this->print_error_location(msg_Warning+": doubled float_litral points", "\e[1;33m");
									std::cout << "\t\e[1;36mthe lexer will fix it automaticly\e[0m\n\n";
									next();
								} else {
									if (inpos() == '.' and !is_float) is_float = true;
									addchar(next());
								}
							}
							this->m_i--;

							if (is_float) {
								ret.add(this->getk("#float_lit"));
								is_float = false;
							}
							else
								ret.add(this->getk("#int_lit"));
						}

						// get a KEY_WORDs that are All Start With '/'
						else if (c == '/') {
							this->m_i++;
							if (std::isalnum(inpos()) or inpos() == '_')
								goto add_ident;
							else if (inpos() == '-') {
								addchar(next());
								if (inpos() == '>') addchar(next());
								else {
									this->print_error_location(msg_Warning+": faurget the '>' after '/-' define of program Entry-Point", "\e[1;33m");
									std::cerr << "\t\e[1;36mthe lexer will fix that automaticly\e[0m\n\n";
									addchar('>');
								}
							}
							else if (inpos() == '/') {
								for (;is_peeked(next() != '\n');)
								  if (inpos() == '\n') {
									this->word.clear();
									goto add_newline;
								  }
							}
							else if (inpos() == '*') {
								this->m_i++;
								for (;;) if (is_peeked(next() == '*' and next() == '/')) {
									this->word.clear();
									break;
								} else if (inpos() == '\n') this->lines_num++;
								this->m_i--;
								continue;
							}
							this->m_i--;
							if (this->ucean_kw.contains(this->word)) {
								token_t tk_= this->getk();
								if (tk_.type == this->ucean_kw["/->"]) {
									static token_t main_function_first_token;
									if (main_function_first_token.type == this->ucean_kw["/->"]) {
										if constexpr (AUTODBG) {
											this->print_error_location(msg_Warning+": the programe Entry-Point is already declared", "\e[1;33m");
											this->setup_token_for_error(main_function_first_token);
											this->print_error_location(msg_Node+": declare in", "\e[1;36m");
											this->setup_token_for_error(tk_); {
												auto tks= this->getks_ov({"/fn", "#ident", "(", ")"});
												tks[1].value= "main";
												ret.add(tks);
											}
											std::cerr << "\t\e[1;36mthe lexer will add \e[1;32m'/fn main()'\e[1;36m instead of it\e[0m\n";
										}
										else {
											this->print_error_location(msg_Error+": the programe 'EntryPoint' is already declared");
											this->setup_token_for_error(main_function_first_token);
											this->print_error_location(msg_Node+": declare in", "\e[1;36m");
											this->setup_token_for_error(tk_);
										}
									} else ret.add(main_function_first_token=tk_);
								} else ret.add(tk_);
							} else unexisted_kw_err(ret);
						}

						// get a KEY_WORDs that are All Start With '/'
						else if (c == '#') {
							this->m_i++;
							if (std::isalnum(inpos()) or inpos() == '_')
								goto add_ident;
							else if (inpos() == '-') {
								addchar(next());
								if (inpos() == '>') addchar(next());
								else {
									this->print_error_location(msg_Warning+": faurget the '>' after '#-' define of program Start-Init-Entry-Point", "\e[1;33m");
									std::cerr << "\t\e[1;36mthe lexer will fix that automaticly\e[0m\n\n";
									addchar('>');
								}
							}
							this->m_i--;
							if (this->ucean_kw.contains(this->word)) ret.add(this->getk());
							else unexisted_kw_err(ret);
						}

						// get a OPERATIONS
						else if (isoprtr(c)) {
							ret.add(this->getk());
							this->m_i++;
							while ( is_peeked(isoprtr(inpos())) ) {
								addchar(next());
								ret.add(this->getk());
							}
							this->m_i--;
						}
					} if (!this->word.empty()) this->word.clear();
				}
			}
			ret.add(token_t(ucean_kw["\\n"]));
			return ret;
		}



	protected:
		std::size_t m_i=0, lines_num= 0;
		str_t m_data, word;

		inline char inpos  (std::size_t i=0) { return this->m_data[this->m_i+i]; }
		inline char previos() { return this->m_data.at(this->m_i--); }
		inline char next   () { return this->m_data.at(this->m_i++); }

		inline void addchar(const char& c) { this->word += c; }

		inline bool is_peeked(bool ret) { return ret and this->peek().has_value(); }

		template<std::size_t ahead=1> [[nodiscard]]
		inline std::optional<char> peek() {
			if (this->m_i + ahead >= this->m_data.length()) return {};
			return this->m_data.at(this->m_i);
		}



		inline void back_inline_to_add(Arrays::Array<token_t> &ret, std::vector<token_t> addeds) {
			Arrays::Array<token_t> shifted_elements;
			int tabs = 0;
			for (std::size_t i=ret.size()-1; i>0; i--) {
				if (ret[i].type == 0) {
					for (int i=0; i<tabs; i++) ret.add(token_t(ucean_kw["\\t"]));
					ret.add(addeds);
					ret.add(token_t(ucean_kw["\\n"]));
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
				this->print_error_location(msg_Warning+": there is no keyword call \e[1;31m'" + this->word + "'\e[0m select one with", "\e[1;33m");
				for (str_t& kw : this->ucean_kw) if (this->word.contains(kw) and kw.cmp("/")) {
					std::cout << "the token \e[1;31m'"<<this->word<<"'\e[0m are contains the keyword \e[1;32m'"<<kw
						<< "'\e[0m do you want to replace it with this keyword [Y/n/R]?\n"
						<< "\e[1;30myou have to answer [Yes/No/Replace With]\e[0m :";
					char c;
					std::cin >> c;
					if (c == 'y' or c == 'Y') {
						this->word.clear();
						this->word += kw;
						ret.add(this->getk());
						break;
					} else if (c == 'y' or c == 'Y') {
						outn("\e[1;30mEnter the keyword that you want:\e[0m")
						this->word.clear();
						std::getline(std::cin, this->word);
						ret.add(this->getk());
						break;
					}
				}
			} else
				this->print_error_location(msg_Error+": there is no keyword call \e[1;31m'" + this->word + "'\e[0m plase fix that");
		}

		inline std::size_t get_tab_level(Arrays::Array<token_t> &ret) {
			std::size_t ret_;
			for (std::size_t i=ret.size()-1; i>0; i--)
				if	  (ret[i].type == 0) break;
				else if (ret[i].type == 1) ret_++;
			return ret_;
		}
	};
}
