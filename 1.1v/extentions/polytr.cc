export module skbase.polytr;

import std;

export template<typename polyextend_t>
class Polymorphic {
private :
	polyextend_t* p;
public :
	template<typename poly_t> 
	Polymorphic(poly_t* p) :p(static_cast<polyextend_t*>(p)) {}

	inline auto operator -> () { return p; }
};
