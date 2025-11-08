#pragma once

#define __SK_LIB_VERSION__ "0.9.6"

#define woutn(arg) std::wcout << arg << std::endl;
#define outn(arg) std::cout << arg << std::endl;
#define putn(arg) std::cin >> arg;

#define ovrd override
#define abstr virtual

// default Setting and Getting
#define sget_addition(type, name)                                \
	inline void set_##name(type name) { this->name = name; }    \
    inline type get_##name() { return this->name; }


/* Array addition : 
    * toarray, fromarray
    * toarraytext, fromarraytext
    * toarrayfile, fromarrayfile
*/
#define array_addition(type, save, read, DRW, abstraction, ovrding)                                         \
    abstraction std::vector<type> toarray() ovrding { save }                                               \
    abstraction void fromarray(std::vector<type> array_) ovrding { read }                                  \
    abstraction std::vector<std::string> tostringarray(std::function<std::string(type)> tr DRW) ovrding {  \
        std::vector<std::string> ret, th_arr = toarray();                                                  \
        for(type i : th_arr) ret.push_back(tr(i));                                                         \
        return ret; }



/* Tuple addition
    * Integrated|use with|as tuple
*/

#define tuple_addition(block)                        \
    template <size_t Index> auto get() const {block}

#define tuple_addition_p2(Type, size, tupletype)                                       \
    namespace std {                                                                   \
        template<> struct tuple_size<Type> : std::integral_constant<size_t, size> {}; \
        template<size_t Index> struct tuple_element<Index, Type> {                    \
            using type = tupletype;                                                   \
        };                                                                            \
    }



#define ucc_panel(name, text) \
	if (ucc::ui::panel_t name(text); name())
#define ucc_panel_(name, text, rect) \
	if (ucc::ui::panel_t name(text); name(rect))
#define ucc_panel__(name, text, rect, flags) \
	if (ucc::ui::panel_t name(text); name(rect, flags))



#define msg_Warning str_t("\e[1;33mWarning\e[0m")
#define msg_Error   str_t("\e[1;31mError\e[0m")
#define msg_Node    str_t("\e[1;36mNode\e[0m")
