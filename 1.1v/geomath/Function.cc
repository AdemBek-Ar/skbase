export module skbase.geomath.math;

import skbase.json;
import skbase.base;
import skbase.memory.array;
import std;

#include "../defines.h++"



export namespace math{
	template <typename num_t=float> class Function {
	public :
		num_t (*y)(num_t);
		Function(num_t (*y)(num_t)= [](num_t x){ return x; }) :y(y) {}

		template<num_t step=1> inline Arrays::Array<num_t> range(num_t start=0, num_t end=10) {
			Arrays::Array<num_t> ret;
			for (num_t x=start; x<end; x+=step) ret.add(this->y(x));
			return ret;
		}
		inline Arrays::Array<num_t> range(std::vector<num_t> Xs) {
			Arrays::Array<num_t> ret;
			for (num_t x : range) ret.add(this->y(x));
			return ret;
		}
		inline Arrays::Array<num_t> range(num_t* Xs) {
			Arrays::Array<num_t> ret;
			for (num_t i=0, size=num_t(sizeof(range)/sizeof(num_t)); i<size; i++) ret.add(this->y(Xs[i]));
			return ret;
		}

		static inline Function E() requires std::is_same_v<num_t, double> { return [](const num_t& x) { return long(x); }; }
		static inline Function E() requires std::is_same_v<num_t, float>  { return [](const num_t& x) { return int (x); }; }
		static inline Function pow    (const num_t& n=2) { return [n](num_t x) { return num_t(pow (x,n)); }; }
		static inline Function i_mul_x(const num_t& i=2) { return [i](num_t x) { return num_t(     x*i ); }; }
		static inline Function squr() { return [](const num_t& x) { return num_t(squr(x)); }; }
		static inline Function cos () { return [](const num_t& x) { return num_t(cos (x)); }; }
		static inline Function sin () { return [](const num_t& x) { return num_t(sin (x)); }; }
		static inline Function tan () { return [](const num_t& x) { return num_t(tan (x)); }; }
		static inline Function acos() { return [](const num_t& x) { return num_t(acos(x)); }; }
		static inline Function asin() { return [](const num_t& x) { return num_t(asin(x)); }; }
		static inline Function atan() { return [](const num_t& x) { return num_t(atan(x)); }; }
	};



	namespace Number{
		template<typename num_t> requires std::is_arithmetic_v<num_t>
		num_t to_num(const std::string& s) {
			num_t value{};
			std::from_chars(s.data(), s.data() + s.size(), value);
			return value;
		}
		template<typename num_t> requires std::is_arithmetic_v<num_t>
		num_t to_num(const std::wstring& ws)
			{ return to_num<num_t>(std::string(ws.begin(), ws.end())); }
	}
}
