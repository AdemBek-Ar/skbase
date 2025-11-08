module;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>
#include <GL/glu.h>

export module skbase.geomath.point;

import skbase.json;
import skbase.base;
import skbase.memory.array;
import skbase.geomath.math;
import std;

#include "../defines.h++"

namespace file {
	extern std::ofstream  save        (const char*    filename, const char*    src);
	extern std::string    get_contents(const char*    filename                    );
}

#include "../extentions/str/wstrbase.h++"
#include "../extentions/str/strbase.h++"



export namespace geometry {
	// custem dimensional @c Rectangle class
	template<typename num_t, std::size_t size> class Rect;

	// use Rectangle custem class to make shortcuts for eche dimention
	template<typename num_t> using Rect2D = Rect<num_t, 2>;
	template<typename num_t> using Rect3D = Rect<num_t, 3>;
	template<typename num_t> using Rect4D = Rect<num_t, 4>;
	using RectF2D = Rect<float, 2>;
	using RectF3D = Rect<float, 3>;
	using RectF4D = Rect<float, 4>;


	// Point class main functions map
	/*	variables : x, y, in 3d : z, in 3d and 4d : w
	*		 for more than 4d it's will be a data array with out custem names for eche variable

	*	functions :

	*		trans(form|to) string	: tostring, fromstring, tobytes, frombyte, tofile(str), fromfile(str), print,

	*		trans(form|to) std::wstring	: towstring, fromwstring, wprint,
	*		trans(form|to) json		: tojson, fromjson, tojsonfile, fromjsonfile, printjson,
	*		math operations			: length, dot, cross, angle_to, distance_to,
	*		addotions				 : mirror, is_inside, to_glmvec
	*/;

	// custem noramls extantion for Point class
	template<typename num_t = float, std::size_t Size = 2, bool Enable = false> class PointNoramls_extention;
	// custem texture coorders extantion for Point class
	template<typename num_t = float, bool Enable = false> class PointTexcoords_extention;
	// custem color extantion for Point class
	template<typename num_t = float, bool Enable = false> class PointColor_extention;

	// unactivete extantions
	template<typename num_t, std::size_t Size> class PointNoramls_extention<num_t, Size, false> {};
	template<typename num_t> class PointTexcoords_extention<num_t, false> {};
	template<typename num_t> class PointColor_extention<num_t, false> {};

	// custem dimentional basice Point struct
	template<typename num_t, std::size_t Size> class Point_struct;
	// basice Point struct for 2d
	template<typename num_t> class Point_struct<num_t, 2> { 
		public : union {
			// position union
			struct { num_t x, y; };
			// color union
			struct { num_t r, g; };
			// dynamice array union
			num_t data[2];
		};
		constexpr Point_struct(std::initializer_list<num_t> init) {
			auto it = init.begin();
			for (std::size_t i = 0; i < 2 && i < init.size(); ++i) data[i] = *it++;
		}
		Point_struct(num_t x = 0, num_t y = 0)
			:x(x), y(y) {}
		Point_struct(num_t* data) :
			x(data[0]), y(data[1]) {}
	};
	// basice Point struct for 3d
	template<typename num_t> class Point_struct<num_t, 3> { 
		public : union {
			// position union
			struct { num_t x, y, z; };
			// color union
			struct { num_t r, g, b; };
			// dynamice array union
			num_t data[3];
		}; 
		constexpr Point_struct(std::initializer_list<num_t> init) {
			auto it = init.begin();
			for (std::size_t i = 0; i < 3 && i < init.size(); ++i) data[i] = *it++;
		}
		Point_struct(num_t x = 0, num_t y = 0, num_t z = 0)
			:x(x), y(y), z(z) {}
		Point_struct(num_t* data)
			:x(data[0]), y(data[1]), z(data[2]) {}
	};
	// basice Point struct for 4d
	template<typename num_t> class Point_struct<num_t, 4> { 
		public : union {
			// position union
			struct { num_t x, y, z, w; };
			// color union
			struct { num_t r, g, b, a; };
			// dynamice array union
			num_t data[4];
		};
		constexpr Point_struct(std::initializer_list<num_t> init) {
			auto it = init.begin();
			for (std::size_t i = 0; i < 4 && i < init.size(); ++i) data[i] = *it++;
		}
		Point_struct(num_t x = 0, num_t y = 0, num_t z = 0, num_t w = 0)
			:x(x), y(y), z(z), w(w) {}
		Point_struct(num_t* data)
			:x(data[0]), y(data[1]), z(data[2]), w(data[2]) {}
	};
	// basice Point struct for mach than >4d
	template<typename num_t, std::size_t Size> class Point_struct { public : num_t data[Size]; };



	// the main custemazed Point class
	template<
		typename num_t,
		std::size_t Size,
		bool is_colored = false,
		bool has_noramls = false,
		bool is_textured = false>
	requires (Size > 0 && std::is_arithmetic_v<num_t>)
	class Point :
		public Point_struct<num_t, Size>,
		public PointNoramls_extention<num_t, 2, has_noramls>,
		public PointTexcoords_extention<num_t, is_textured>,
		public PointColor_extention<num_t, is_colored> {
	public :
		constexpr Point(std::initializer_list<num_t> init) : Point_struct<num_t, Size>(init) {}
		Point(num_t x = 0, num_t y = 0, num_t z = 0, num_t w = 1) requires (Size == 4) : Point_struct<num_t, Size>(x, y, z, w) {}
		Point(num_t x = 0, num_t y = 0, num_t z = 0) requires (Size == 3) : Point_struct<num_t, Size>(x, y, z) {}
		Point(num_t x = 0, num_t y = 0) requires (Size == 2) : Point_struct<num_t, Size>(x, y) {}
		Point(num_t* data) {
			int i = 0;
			for (; i < int(Size); i++) this->data[i] = data[i];
			i = Size;
			if constexpr(has_noramls) {
				for (int i_Size = int(Size) * 2; i < i_Size; i++) this->noramls.data[i] = data[i];
				i = Size * 2;
			}
			if constexpr(is_textured) {
				for (int j = 0; i < 2; j++) this->texcoords.data[j] = data[i + j];
				i += 2;
			}
			if constexpr(is_colored) for (int j = 0; i < 4; j++) this->color.data[j] = data[i + j];
		}

		template<typename num_T> 
		using this_cast_t = Point<num_T, Size, is_colored, has_noramls, is_textured>;
		using this_t = Point<num_t, Size, is_colored, has_noramls, is_textured>;

		inline operator Rect2D<num_t>() const requires (Size == 4) { return {this->x, this->y, this->z, this->w}; }
		inline Rect2D<num_t> to_rect() requires(Size == 4) { return {this->x, this->y, this->z, this->w}; }
		
		inline bool is_inside_rect(num_t x, num_t y, num_t z, num_t w, num_t wd, num_t h, num_t l, num_t li) requires (Size == 4) { return this->x > x and this->y > y and this->z > z and this->w > w and this->x < wd and this->y < h and this->z < l and this->w < li; }
		inline bool is_inside_rect(Point<num_t, 4> pos, Point<num_t, 4> size) requires (Size == 4) { return is_inside_rect(pos.x, pos.y, pos.z, pos.w, size.x, size.y, size.z, size.w); }

		inline bool is_inside_rect(num_t x, num_t y, num_t z, num_t w, num_t h, num_t l) requires (Size == 3) { return this->x > x and this->y > y and this->z > z and this->x < w and this->y < h and this->z < l; }
		inline bool is_inside_rect(Point<num_t, 3> pos, Point<num_t, 3> size) requires (Size == 3) { return is_inside_rect(pos.x, pos.y, pos.z, size.x, size.y, size.z); }

		inline bool is_inside_rect(num_t x, num_t y, num_t w, num_t h) requires (Size == 2) { return this->x > x and this->y > y and this->x < w and this->y < h; }
		inline bool is_inside_rect(Point<num_t, 2> pos, Point<num_t, 2> size) requires (Size == 2) { return is_inside_rect(pos.x, pos.y, size.x, size.y); }

		inline bool is_inside_rect(Rect<num_t, Size> rect) {

			bool ret = true;
			for (std::size_t i = 0; i < Size; i++) if (this->data[i] < rect.data[i]) ret = false;
			for (std::size_t i = Size, DSize = Size * 2; i < DSize; i++) if (this->data[i / 2] > rect.data[i]) ret = false;
			return ret;
		}

		inline bool is_inside_circle(Point<num_t, 3> pos, Point<num_t, 3> size) requires (Size == 3) { return is_inside_circle(pos.x, pos.y, pos.z, size.x, size.y, size.z); }
		inline bool is_inside_circle(num_t x, num_t y, num_t z, num_t rx, num_t ry = -1, num_t rz = -1) requires (Size == 3) {
			if (ry == -1) ry = rx;
			if (rz == -1) rz = ry;
			return rx > (this->x - x) and ry > (this->y - y) and rz > (this->z - z);
		}

		inline bool is_inside_circle(Point<num_t, 4> pos, Point<num_t, 4> size) requires (Size == 4) { return is_inside_circle(pos.x, pos.y, pos.z, size.x, size.y, size.z); }
		inline bool is_inside_circle(num_t x, num_t y, num_t z, num_t w, num_t rx, num_t ry = -1, num_t rz = -1, num_t rw = -1) requires (Size == 4) {
			if (ry == -1) ry = rx;
			if (rz == -1) rz = ry;
			if (rw == -1) rw = rz;
			return rx > (this->x - x) and ry > (this->y - y) and rz > (this->z - z) and rw > (this->w - w);
		}

		inline bool is_inside_circle(Point<num_t, 2> pos, Point<num_t, 2> size) requires (Size == 2) { return is_inside_circle(pos.x, pos.y, size.x, size.y); }
		inline bool is_inside_circle(num_t x, num_t y, num_t rx, num_t ry = -1) requires (Size == 2) {
			if (ry == -1) ry = rx;
			return rx > (this->x - x) and ry > (this->y - y);
		}

		inline bool is_inside_circle(Rect<num_t, Size> rect) {
			bool ret = true;
			for (int i = Size; i < Size * 2; i++) if (rect.data[i] < (this->data[i / 2] - rect.data[i / 2])) ret = false;
			return ret;
		}

		inline num_t length() {
			num_t ret = 0;
			for (int i = 0; i < int(Size); i++) ret += this->data[i] * this->data[i];
			return (num_t)sqrt(ret);
		}

				template<char opr='+'> inline num_t dimopr() {
					num_t ret = this->data[0];
					if	  constexpr (opr == '-') for (std::size_t i=1; i < Size; i++) ret -= this->data[i];
					else if constexpr (opr == '*') for (std::size_t i=1; i < Size; i++) ret *= this->data[i];
					else if constexpr (opr == '/') for (std::size_t i=1; i < Size; i++) ret /= this->data[i];
					else if constexpr (opr == '^') for (std::size_t i=1; i < Size; i++) ret  = num_t(pow(ret, this->data[i]));
					else						   for (std::size_t i=1; i < Size; i++) ret += this->data[i];
					return ret;
				}

		Point<num_t, Size> mirror(bool mirror[Size], bool set_this_to = false) const {
			Point<num_t, Size> ret;
			for (int i = 0; i < int(Size); i++)
				if (set_this_to) ret.data[i] = (this->data[i] = -this->data[i]);
				else ret.data[i] = -this->data[i];
			return ret;
		}
		Point mirror(bool mirror_x, bool mirror_y, bool mirror_z, bool mirror_w, bool set_this_to) const requires (Size == 4) {
			return Point<num_t, Size>{
				mirror_x ? (set_this_to ? this->x = -this->x : -this->x) : this->x,
				mirror_y ? (set_this_to ? this->y = -this->y : -this->y) : this->y,
				mirror_z ? (set_this_to ? this->z = -this->z : -this->z) : this->z,
				mirror_w ? (set_this_to ? this->w = -this->w : -this->w) : this->w
			};
		}
		Point mirror(bool mirror_x, bool mirror_y, bool mirror_z, bool set_this_to) const requires (Size == 3) {
			return Point<num_t, Size>{
				mirror_x ? (set_this_to ? this->x = -this->x : -this->x) : this->x,
				mirror_y ? (set_this_to ? this->y = -this->y : -this->y) : this->y,
				mirror_z ? (set_this_to ? this->z = -this->z : -this->z) : this->z
			};
		}
		Point mirror(bool mirror_x, bool mirror_y, bool set_this_to) const requires (Size == 2) {
			return Point<num_t, Size>{
				mirror_x ? (set_this_to ? this->x = -this->x : -this->x) : this->x,
				mirror_y ? (set_this_to ? this->y = -this->y : -this->y) : this->y
			};
		}
		Point mirror(bool mirror = true, bool set_this_to = false) const {
			Point<num_t, Size> ret;
			for (int i = 0; i < int(Size); i++) if (mirror) ret.data[i] = set_this_to ? this->data[i] = -this->data[i] : -this->data[i];
			return ret;
		}

		Point<num_t, Size> mid_point(const Point<num_t, Size>& pos, bool set_this_to = false) {
			Point<num_t, Size> ret;
			for (int i = 0; i < int(Size); i++)
				if (set_this_to) ret.data[i] = (this->data[i] = (this->data[i] + pos.data[i]) / 2);
				else ret.data[i] = ((this->data[i] + pos.data[i]) / 2);
			return ret;
		}

		num_t dot(const Point<num_t, Size>& other) const {
			num_t ret = 0;
			for (int i = 0; i < int(Size); i++) ret += this->data[i] * other.data[i];
			return ret;
		}

		num_t cross(const Point<num_t, 2>& other) const requires (Size == 2) { return this->x * other.y - this->y * other.x; }
		Point<num_t, Size> cross(const Point<num_t, Size>& other) const requires (Size == 3) {
			return Point<num_t, Size>(
				this->y * other.z - this->z * other.y,
				this->z * other.x - this->x * other.z,
				this->x * other.y - this->y * other.x
			);
		}
		Point<num_t, Size> cross(const Point<num_t, Size>& other) const requires (Size == 4) {
			return Point<num_t, Size>(
				this->y * other.z - this->z * other.y,
				this->z * other.x - this->x * other.z,
				this->x * other.y - this->y * other.x,
				this->x * other.y - this->y * other.x
			);
		}

		num_t angle_to(const num_t& ox, const num_t& oy, const num_t& oz, const num_t& ow) const requires (Size == 3) { return this->angle_to({ ox, oy, oz, ow }); }
		num_t angle_to(const num_t& ox, const num_t& oy, const num_t& oz) const requires (Size == 3) { return this->angle_to({ ox, oy, oz }); }
		num_t angle_to(const num_t& ox, const num_t& oy) const requires (Size == 2) { return this->angle_to({ ox, oy }); }
		num_t angle_to(const Point<num_t, Size>& other) const {
			num_t dot_ = this->dot(other), th_len = this->length(), oth_len = other.length();
			if (th_len < (1e-7) or oth_len < (1e-7)) throw std::invalid_argument("the std::vector is 'Zero std::vector'");
			num_t alg_cos = dot_ / (th_len * oth_len); alg_cos = num_t(max(-1.0, min(1.0, alg_cos)));
			return num_t(acos(alg_cos) * 180 / M_PI);
		}

		num_t distance_to(const Point<num_t, 2>& other) const {
			num_t ret = 0;
			for (int i = 0; i < int(Size); i++) {
				num_t di = this->data[i] - other.data[i];
				ret += di * di;
			} return (num_t)sqrt(ret);
		}

		template<typename new_num_t, std::size_t new_size = Size, new_num_t default_value = static_cast<new_num_t>(0)> Point<new_num_t, new_size, is_colored, has_noramls, is_textured> cast() const {
			Point<new_num_t, new_size, is_colored, has_noramls, is_textured> ret;
			if (Size == new_size) {
			  std::size_t bigger = (Size > new_size) ? Size : new_size,
					 minuer = (Size > new_size) ? new_size : Size;
			  for (int i = 0; i < int(bigger); i++)
				if (i < static_cast<int>(minuer)) ret.data[i] = static_cast<new_num_t>(this->data[i]);
				else ret.data[i] = default_value;
			} else for (int i = 0; i < int(Size); i++) ret.data[i] = static_cast<new_num_t>(this->data[i]);
			return ret;
		}

		inline std::string tostring(std::string spliter = ",") {
			std::string ret;
			for (int i = 0; i < int(Size); i++) {
				ret += std::to_string(this->data[i]);
				if (i != Size - 1) ret += spliter;
			}
			if constexpr(has_noramls) ret += spliter + this->noramls.tostring(spliter);
			if constexpr(is_textured) ret += spliter + this->texcoords.tostring(spliter);
			if constexpr(is_colored) ret += spliter + this->color.tostring(spliter);
			return ret;
		}
		inline void fromstring(std::string str, std::string spliter = ",") {
			std::vector<std::string> strs = str::split(str, spliter);
			std::size_t num_arrow = 0;
			for (std::size_t i=0; i < Size; i++)
				this->data[i]= math::Number::to_num<num_t>(strs[i]);
			num_arrow = Size;
			if constexpr (has_noramls) {
				for (std::size_t i=Size; i < Size*2; i++)
					this->noramls.data[i/2]= math::Number::to_num<num_t>(strs[i]);
				num_arrow = 2*Size;
			}
			if constexpr (is_textured) {
				this->texcoords= Point<num_t, 2>(
					math::Number::to_num<num_t>(strs[num_arrow]),
					math::Number::to_num<num_t>(strs[num_arrow+1]));
				num_arrow += 2;
			}
			if constexpr (is_colored) {
				this->color= Point<num_t, 4>(
					math::Number::to_num<num_t>(strs[num_arrow]),
					math::Number::to_num<num_t>(strs[num_arrow+1]),
					math::Number::to_num<num_t>(strs[num_arrow+2]),
					math::Number::to_num<num_t>(strs[num_arrow+3]));
			}
		}
		
		inline std::vector<byte> tobytes(std::string spliter = ",") {
			std::vector<byte> ret;
			std::string th_str = tostring();
			for (char ch : th_str) ret.push_back(static_cast<byte>(ch));
			return ret;
		}
		inline void frombytes(std::vector<byte> bytes, std::string spliter = ",") {
			std::string reading_from = "";
			for (byte b : bytes) reading_from += static_cast<char>(b);
			fromstring(reading_from);
		}
		
		inline void tofile(std::string path, std::string spliter = ",") { file::save(path.c_str(), tostring().c_str()); }
		inline void fromfile(std::string path, std::string spliter = ",") { fromstring(file::get_contents(path.c_str()), spliter); }
		
		inline void print() { std::cout << tostring() << std::endl; }

		inline std::wstring towstring(std::wstring spliter = L",") {
			std::wstring ret;
			for (int i = 0; i < int(Size); i++) {
				ret += std::to_wstring(this->data[i]);
				if (i != Size - 1) ret += spliter;
			}
			if constexpr(has_noramls) ret += spliter + this->noramls.towstring(spliter);
			if constexpr(is_textured) ret += spliter + this->texcoords.towstring(spliter);
			if constexpr(is_colored) ret += spliter + this->color.towstring(spliter);
			return ret;
		}
		inline void fromwstring(std::wstring str, std::wstring spliter = L",") {
			std::vector<std::wstring> strs = str::split(str, spliter);
			for (int i = 0; i < int(Size); i++) this->data[i] = math::Number::to_num<num_t>(strs[i]);
			int rsize_ = Size * 2;
			if constexpr(has_noramls) for (int i = Size; i < rsize_; i++) this->noramls.data[i / 2] = math::Number::to_num<num_t>(strs[i]);
			if constexpr(is_textured) this->texcoords = Point<num_t, 2>(math::Number::to_num<num_t>(strs[rsize_]), math::Number::to_num<num_t>(strs[rsize_ + 1]));
			if constexpr(is_colored) this->color = Point<num_t, 4>(math::Number::to_num<num_t>(strs[rsize_ + 2]), math::Number::to_num<num_t>(strs[rsize_ + 3]), math::Number::to_num<num_t>(strs[rsize_ + 4]), (num_t)math::Number::to_num<num_t>(strs[rsize_ + 5]));
		}

		inline void wprint() { std::cout << tostring() << std::endl; }

		inline json tojson() {
			json j;
			if constexpr (Size >= 2 and Size < 5) {
				j["x"] = this->x;
				j["y"] = this->y;
				if constexpr (Size >= 3) j["z"] = this->z;
				if constexpr (Size >= 4) j["w"] = this->w;
			} else for (int i = 0 ; i < Size ; i++) j[std::to_string(i)] = this->data[i];
			if constexpr (has_noramls) j["noramls"] = this->noramls.tojson();
			if constexpr(is_textured) j["texcoords"] = this->texcoords.tojson();
			if constexpr (is_colored) j["color"] = this->color.tojson();
			return j;
		}
		inline void fromjson(json j) {
			if constexpr (Size >= 2 and Size < 5) {
				this->x = j["x"];
				this->y = j["y"];
				if constexpr (Size >= 3) this->z = j["z"];
				if constexpr (Size >= 4) this->w = j["w"];
			} else for (int i = 0 ; i < Size ; i++) this->data[i] = j[std::to_string(i)];
			if constexpr (has_noramls) this->noramls.fromjson(j["noramls"]);
			if constexpr(is_textured) this->texcoords.fromjson(j["texcoords"]);
			if constexpr(is_colored) this->color.fromjson(j["color"]);
		}
		
		inline std::string tojsonstr() { return tojson().dump(4); }
		inline void fromjsonstr(std::string str) {
			json j = json::parse(str);
			fromjson(j);
		}
		
		inline std::ofstream tojsonfile(std::string path) { return file::save(path.c_str(), tojsonstr().c_str()); }
		inline void fromjsonfile(std::string path) { fromjsonstr(file::get_contents(path.c_str())); }

		num_t operator[](const std::size_t& i) {
			std::size_t bm = 0;
			if (i < Size) return this->data[i];
			if constexpr(has_noramls) {
				bm += Size;
				if (i - bm < Size) return this->noramls[i - bm];
			}
			if constexpr(is_textured) {
				bm += Size;
				if (i - bm <= 1) return this->texcoords[i - bm];
			}
			if constexpr(is_colored) {
				bm += 2;
				if (i - bm <= 3) return this->color[i - bm];
			} return this->length();
		}

		template <std::size_t Index> auto get() const { this->operator[](Index); }
		std::vector<num_t> toarray() {
			Arrays::Array<num_t> ret;
			for (int i = 0; i < int(Size); i++) ret.add(this->data[i]);
			if constexpr(has_noramls) for (int i = 0; i < int(Size); i++) ret.add(this->noramls[i]);
			if constexpr(is_textured) for (int i = 0; i < 2; i++) ret.add(this->texcoords[i]);
			if constexpr(is_colored) for (int i = 0; i < 4; i++) ret.add(this->color[i]);
			return ret;
		}
		void fromarray(std::vector<num_t> array_) {
			for (int i = 0; i < int(Size); i++) this->data[i] = array_[i];
			int bm = Size * 2;
			for (int i = Size; i < bm; i++) this->noramls[i] = array_[i];
			if constexpr(has_noramls) this->noramls = { array_[bm++], array_[bm++] };
			if constexpr(is_textured) this->texcoords = { array_[bm++], array_[bm++] };
			if constexpr(is_colored) this->color = { array_[bm++], array_[bm++], array_[bm++], array_[bm] };
		}
		std::vector<std::string> tostringarray(std::function<std::string(num_t)> tr = [](num_t num) { return std::to_string(num); }) {
			std::vector<num_t> this_as_array = this->toarray(), ret = {};
			for (num_t i : this_as_array) ret.push_back(tr(i));
			return ret;
		}

		Point rot(Point<num_t, 2> c, num_t alg, bool apply_at_this = true) requires (Size == 2) { return rot(c.x, c.y, alg, apply_at_this); }
		Point rot(num_t cx, num_t cy, num_t alg, bool apply_at_this = true) requires (Size == 2) {
			num_t alg_by_rad = num_t(alg * M_PI / 180), alg_cos = num_t(std::cos(alg_by_rad)), alg_sin = num_t(std::sin(alg_by_rad));
			Point<num_t, 2> d = this->operator-({ cx, cy });
			if (apply_at_this) return Point<num_t, 2>(this->x = d.x * alg_cos - d.y * alg_sin, this->y = d.x * alg_sin + d.y * alg_cos);
			return Point<num_t, 2>(d.x * alg_cos - d.y * alg_sin, d.x * alg_sin + d.y * alg_cos);
		}

		inline Point rotX(Point<num_t, 3> c, num_t alg, bool apply_at_this = true) requires (Size == 3) { return rotX(c.x, c.y, c.z, alg, apply_at_this); }
		inline Point rotX(num_t cx, num_t cy, num_t cz, num_t alg, bool apply_at_this = true) requires (Size == 3) {
			num_t alg_by_rad = num_t(alg * M_PI / 180), alg_cos = num_t(std::cos(alg_by_rad)), alg_sin = num_t(std::sin(alg_by_rad));
			Point<num_t, 3> d = this->operator-({ cx, cy, cz });
			if (apply_at_this) return Point<num_t, 3>(
				this->x,
				this->y = d.y * alg_cos - d.z * alg_sin,
				this->z = d.y * alg_sin + d.z * alg_cos
			); return Point<num_t, 3>(
				this->x,
				d.y * alg_cos - d.z * alg_sin,
				d.y * alg_sin + d.z * alg_cos
			);
		}

		inline Point rotY(Point<num_t, 3> c, num_t alg, bool apply_at_this = true) requires (Size == 3) { return rotY(c.x, c.y, c.z, alg, apply_at_this); }
		inline Point rotY(num_t cx, num_t cy, num_t cz, num_t alg, bool apply_at_this = true) requires (Size == 3) {
			num_t alg_by_rad = num_t(alg * M_PI / 180), alg_cos = num_t(std::cos(alg_by_rad)), alg_sin = num_t(std::sin(alg_by_rad));
			Point<num_t, 3> d = this->operator-({ cx, cy, cz });
			if (apply_at_this) return Point<num_t, 3>(
				this->x = d.x * alg_cos + d.z * alg_sin,
				this->y,
				this->z =-d.x * alg_sin + d.z * alg_cos
			); return Point<num_t, 3>(
				d.x * alg_cos + d.z * alg_sin,
				this->y,
				-d.x * alg_sin + d.z * alg_cos
			);
		}

		inline Point rotZ(Point<num_t, 3> c, num_t alg, bool apply_at_this = true) requires (Size == 3) { return rotZ(c.x, c.y, c.z, alg, apply_at_this); }
		inline Point rotZ(num_t cx, num_t cy, num_t cz, num_t alg, bool apply_at_this = true) requires (Size == 3) {
			num_t alg_by_rad = num_t(alg * M_PI / 180), alg_cos = num_t(std::cos(alg_by_rad)), alg_sin = num_t(std::sin(alg_by_rad));
			Point<num_t, 3> d = this->operator-({ cx, cy, cz });
			if (apply_at_this) return Point<num_t, 3>(
				this->x = d.x * alg_cos - d.y * alg_sin,
				this->y = d.x * alg_sin + d.y * alg_cos,
				this->z
			); return Point<num_t, 3>(
				d.x * alg_cos - d.y * alg_sin,
				d.x * alg_sin + d.y * alg_cos,
				this->z
			);
		}

		inline Point rot(Point<num_t, 3> c, num_t alg, bool apply_at_this = true) requires (Size == 3) { return rot(c.x, c.y, c.z, alg, apply_at_this); }
		inline Point rot(num_t cx, num_t cy, num_t cz, num_t alg, bool apply_at_this = true) requires (Size == 3) {
			Point<num_t, 3> ret = rotX(cx, cy, cz, alg, apply_at_this).rotY(cx, cy, cz, alg, apply_at_this).rotZ(cx, cy, cz, alg, apply_at_this);
			if (apply_at_this) { this->x = ret.x; this->y = ret.y; this->z = ret.z; }
			return ret;
		}

		inline Point rot(Point<num_t, 3> c, num_t alg, bool apply_at_this[3] = { true , true ,true }) requires (Size == 3) { return rot(c.x, c.y, c.z, alg, apply_at_this); }
		inline Point rot(num_t cx, num_t cy, num_t cz, num_t alg, bool apply_at_this[3] = { true , true ,true }) requires (Size == 3) {
			Point<num_t, 3> ret = rotX(cx, cy, cz, alg, apply_at_this[0]).rotY(cx, cy, cz, alg, apply_at_this[1]).rotZ(cx, cy, cz, alg, apply_at_this[2]);
			for (int i = 0; i < 3; i++) if (apply_at_this[i]) this->data[i] = ret.data[i];
			return ret;
		}

		inline Point rot(Point<num_t, 3> c, num_t algX, num_t algY, num_t algZ, bool apply_at_this = true) requires (Size == 3) { return rot(c.x, c.y, c.z, algX, algY, algZ, apply_at_this); }
		inline Point rot(num_t cx, num_t cy, num_t cz, num_t algX, num_t algY, num_t algZ, bool apply_at_this = true) requires (Size == 3) {
			Point<num_t, 3> ret = rotX(cx, cy, cz, algX, apply_at_this).rotY(cx, cy, cz, algY, apply_at_this).rotZ(cx, cy, cz, algZ, apply_at_this);
			if (apply_at_this) { this->x = ret.x; this->y = ret.y; this->z = ret.z; }
			return ret;
		}

		inline Point rot(Point<num_t, 3> c, num_t algX, num_t algY, num_t algZ, bool apply_at_this[3] = { true , true ,true }) requires (Size == 3) { return rot(c.x, c.y, c.z, algX, algY, algZ, apply_at_this); }
		inline Point rot(num_t cx, num_t cy, num_t cz, num_t algX, num_t algY, num_t algZ, bool apply_at_this[3] = { true , true ,true }) requires (Size == 3) {
			Point<num_t, 3> ret = rotX(cx, cy, cz, algX, apply_at_this[0]).rotY(cx, cy, cz, algY, apply_at_this[1]).rotZ(cx, cy, cz, algZ, apply_at_this[2]);
			for (int i = 0; i < 3; i++) if (apply_at_this[i]) this->data[i] = ret.data[i];
			return ret;
		}

		Point<num_t, Size> operator++() {
			num_t ret_d[Size];
			for (int i = 0; i < int(Size); i++) { this->data[i]++; ret_d[i] = this->data[i]; }
			return Point<num_t, Size>(ret_d);
		}
		Point<num_t, Size> operator--() {
			num_t ret_d[Size];
			for (int i = 0; i < int(Size); i++) { this->data[i]++; ret_d[i] = this->data[i]; }
			return Point<num_t, Size>(ret_d);
		}
		Point<num_t, Size> operator^=(const Point<num_t, Size>& pos) {
			num_t ret_d[Size];
			for (int i = 0; i < int(Size); i++) ret_d[i] = (this->data[i] = (num_t)pow(this->data[i], pos.data[i]));
			return Point<num_t, Size>(ret_d);
		}
		Point<num_t, Size> operator^=(const num_t& var) {
			num_t ret_d[Size];
			for (int i = 0; i < int(Size); i++) ret_d[i] = (this->data[i] = (num_t)pow(this->data[i], var));
			return Point<num_t, Size>(ret_d);
		}
		Point<num_t, Size> operator+=(const Point<num_t, Size>& pos) {
			num_t ret_d[Size];
			for (int i = 0; i < int(Size); i++) { this->data[i] += pos.data[i]; ret_d[i] = this->data[i]; }
			return Point<num_t, Size>(ret_d);
		}
		Point<num_t, Size> operator+=(const num_t& var) {
			num_t ret_d[Size];
			for (int i = 0; i < int(Size); i++) { this->data[i] += var; ret_d[i] = this->data[i]; }
			return Point<num_t, Size>(ret_d);
		}
		Point<num_t, Size> operator-=(const Point<num_t, Size>& pos) {
			num_t ret_d[Size];
			for (int i = 0; i < int(Size); i++) { this->data[i] -= pos.data[i]; ret_d[i] = this->data[i]; }
			return Point<num_t, Size>(ret_d);
		}
		Point<num_t, Size> operator-=(const num_t& var) {
			num_t ret_d[Size];
			for (int i = 0; i < int(Size); i++) { this->data[i] -= var; ret_d[i] = this->data[i]; }
			return Point<num_t, Size>(ret_d);
		}
		Point<num_t, Size> operator*=(const Point<num_t, Size>& pos) {
			num_t ret_d[Size];
			for (int i = 0; i < int(Size); i++) { this->data[i] *= pos.data[i]; ret_d[i] = this->data[i]; }
			return Point<num_t, Size>(ret_d);
		}
		Point<num_t, Size> operator*=(const num_t& var) {
			num_t ret_d[Size];
			for (int i = 0; i < int(Size); i++) { this->data[i] *= var; ret_d[i] = this->data[i]; }
			return Point<num_t, Size>(ret_d);
		}
		Point<num_t, Size> operator/=(const Point<num_t, Size>& pos) {
			num_t ret_d[Size];
			for (int i = 0; i < int(Size); i++) { this->data[i] /= pos.data[i]; ret_d[i] = this->data[i]; }
			return Point<num_t, Size>(ret_d);
		}
		Point<num_t, Size> operator/=(const num_t& var) {
			num_t ret_d[Size];
			for (int i = 0; i < int(Size); i++) { this->data[i] /= var; ret_d[i] = this->data[i]; }
			return Point<num_t, Size>(ret_d);
		}
		Point<num_t, Size> operator%=(const Point<num_t, Size>& pos) {
			num_t ret_d[Size];
			for (int i = 0; i < int(Size); i++) { this->data[i] %= pos.data[i]; ret_d[i] = this->data[i]; }
			return Point<num_t, Size>(ret_d);
		}
		Point<num_t, Size> operator%=(const num_t& var) {
			num_t ret_d[Size];
			for (int i = 0; i < int(Size); i++) { this->data[i] %= var; ret_d[i] = this->data[i]; }
			return Point<num_t, Size>(ret_d);
		}
		Point<num_t, Size> operator&=(const Point<num_t, Size>& pos) {
			num_t ret_d[Size];
			for (int i = 0; i < int(Size); i++) { this->data[i] &= pos.data[i]; ret_d[i] = this->data[i]; }
			return Point<num_t, Size>(ret_d);
		}
		Point<num_t, Size> operator&=(const num_t& var) {
			num_t ret_d[Size];
			for (int i = 0; i < int(Size); i++) { this->data[i] &= var; ret_d[i] = this->data[i]; }
			return Point<num_t, Size>(ret_d);
		}

		Point<num_t, Size> operator^(const Point<num_t, Size>& pos) const {
			num_t ret_d[Size];
			for (int i = 0; i < int(Size); i++) ret_d[i] = (num_t)pow(this->data[i], pos.data[i]);
			return Point<num_t, Size>(ret_d);
		}
		Point<num_t, Size> operator^(const num_t& var) const {
			num_t ret_d[Size];
			for (int i = 0; i < int(Size); i++) ret_d[i] = (num_t)pow(this->data[i], var);
			return Point<num_t, Size>(ret_d);
		}
		Point<num_t, Size> operator+(const Point<num_t, Size>& pos) const {
			num_t ret_d[Size];
			for (int i = 0; i < int(Size); i++) ret_d[i] = (this->data[i] + pos.data[i]);
			return Point<num_t, Size>(ret_d);
		}
		Point<num_t, Size> operator+(const num_t& var) const {
			num_t ret_d[Size];
			for (int i = 0; i < int(Size); i++) ret_d[i] = (this->data[i] + var);
			return Point<num_t, Size>(ret_d);
		}
		Point<num_t, Size> operator-(const Point<num_t, Size>& pos) const {
			num_t ret_d[Size];
			for (int i = 0; i < int(Size); i++) ret_d[i] = (this->data[i] - pos.data[i]);
			return Point<num_t, Size>(ret_d);
		}
		Point<num_t, Size> operator-(const num_t& var) const {
			num_t ret_d[Size];
			for (int i = 0; i < int(Size); i++) ret_d[i] = (this->data[i] - var);
			return Point<num_t, Size>(ret_d);
		}
		Point<num_t, Size> operator*(const Point<num_t, Size>& pos) const {
			num_t ret_d[Size];
			for (int i = 0; i < int(Size); i++) ret_d[i] = (this->data[i] * pos.data[i]);
			return Point<num_t, Size>(ret_d);
		}
		Point<num_t, Size> operator*(const num_t& var) const {
			num_t ret_d[Size];
			for (int i = 0; i < int(Size); i++) ret_d[i] = (this->data[i] * var);
			return Point<num_t, Size>(ret_d);
		}
		Point<num_t, Size> operator/(const Point<num_t, Size>& pos) const {
			num_t ret_d[Size];
			for (int i = 0; i < int(Size); i++) ret_d[i] = (this->data[i] / pos.data[i]);
			return Point<num_t, Size>(ret_d);
		}
		Point<num_t, Size> operator/(const num_t& var) const {
			num_t ret_d[Size];
			for (int i = 0; i < int(Size); i++) ret_d[i] = (this->data[i] / var);
			return Point<num_t, Size>(ret_d);
		}
		Point<num_t, Size> operator%(const Point<num_t, Size>& pos) const {
			num_t ret_d[Size];
			for (int i = 0; i < int(Size); i++) ret_d[i] = (this->data[i] % pos.data[i]);
			return Point<num_t, Size>(ret_d);
		}
		Point<num_t, Size> operator%(const num_t& var) const {
			num_t ret_d[Size];
			for (int i = 0; i < int(Size); i++) ret_d[i] = (this->data[i] % var);
			return Point<num_t, Size>(ret_d);
		}
		Point<num_t, Size> operator&(const Point<num_t, Size>& pos) const {
			num_t ret_d[Size];
			for (int i = 0; i < int(Size); i++) ret_d[i] = (this->data[i] & pos.data[i]);
			return Point<num_t, Size>(ret_d);
		}
		Point<num_t, Size> operator&(const num_t& var) const {
			num_t ret_d[Size];
			for (int i = 0; i < int(Size); i++) ret_d[i] = (this->data[i] & var);
			return Point<num_t, Size>(ret_d);
		}

		bool operator==(const this_t& pos) const {
			bool ret = true;
			for (std::size_t i=0; i<Size; i++) if (this->data[i] != pos.data[i]) ret = false;
			return ret;
		}
		bool operator!=(const this_t& pos) const {
			bool ret = true;
			for (std::size_t i=0; i<Size; i++) if (this->data[i] == pos.data[i]) ret = false;
			return ret;
		}
		bool operator>=(const this_t& pos) const {
			bool ret = true;
			for (std::size_t i=0; i<Size; i++) if (this->data[i] < pos.data[i]) ret = false;
			return ret;
		}
		bool operator<=(const this_t& pos) {
			bool ret = true;
			for (std::size_t i=0; i<Size; i++) if (this->data[i] > pos.data[i]) ret = false;
			return ret;
		}
		bool operator> (const this_t& pos) const {
			bool ret = true;
			for (std::size_t i=0; i<Size; i++) if (this->data[i] <= pos.data[i]) ret = false;
			return ret;
		}
		bool operator< (const this_t& pos) const {
			bool ret = true;
			for (std::size_t i=0; i<Size; i++) if (this->data[i] >= pos.data[i]) ret = false;
			return ret;
		}

		Point<num_t, Size> operator^=(const glm::tvec4<num_t>& pos) requires (Size == 4) {
			return Point<num_t, Size>(this->x = num_t(pow(this->x, pos.x)), this->y = num_t(pow(this->y, pos.y)), this->z = num_t(pow(this->z, pos.z)), this->w = num_t(pow(this->w, pos.w))); }
		Point<num_t, Size> operator^=(const glm::tvec3<num_t>& pos) requires (Size == 3) {
			return Point<num_t, Size>(this->x = num_t(pow(this->x, pos.x)), this->y = num_t(pow(this->y, pos.y)), this->z = num_t(pow(this->z, pos.z))); }
		Point<num_t, Size> operator^=(const glm::tvec2<num_t>& pos) requires (Size == 2) {
			return Point<num_t, Size>(this->x = num_t(pow(this->x, pos.x)), this->y = num_t(pow(this->y, pos.y))); }
		Point<num_t, Size> operator+=(const glm::tvec4<num_t>& pos) requires (Size == 4) {
			return Point<num_t, Size>(this->x = this->x + pos.x, this->y = this->y + pos.y, this->z = this->z + pos.z, this->w = this->w + pos.w); }
		Point<num_t, Size> operator+=(const glm::tvec3<num_t>& pos) requires (Size == 3) {
			return Point<num_t, Size>(this->x = this->x + pos.x, this->y = this->y + pos.y, this->z = this->z + pos.z); }
		Point<num_t, Size> operator+=(const glm::tvec2<num_t>& pos) requires (Size == 2) {
			return Point<num_t, Size>(this->x = this->x + pos.x, this->y = this->y + pos.y); }
		Point<num_t, Size> operator-=(const glm::tvec4<num_t>& pos) requires (Size == 4) {
			return Point<num_t, Size>(this->x = this->x - pos.x, this->y = this->y - pos.y, this->z = this->z - pos.z, this->w = this->w - pos.w); }
		Point<num_t, Size> operator-=(const glm::tvec3<num_t>& pos) requires (Size == 3) {
			return Point<num_t, Size>(this->x = this->x - pos.x, this->y = this->y - pos.y, this->z = this->z - pos.z); }
		Point<num_t, Size> operator-=(const glm::tvec2<num_t>& pos) requires (Size == 2) {
			return Point<num_t, Size>(this->x = this->x - pos.x, this->y = this->y - pos.y); }
		Point<num_t, Size> operator*=(const glm::tvec4<num_t>& pos) requires (Size == 4) {
			return Point<num_t, Size>(this->x = this->x * pos.x, this->y = this->y * pos.y, this->z = this->z * pos.z, this->w = this->w * pos.w); }
		Point<num_t, Size> operator*=(const glm::tvec3<num_t>& pos) requires (Size == 3) {
			return Point<num_t, Size>(this->x = this->x * pos.x, this->y = this->y * pos.y, this->z = this->z * pos.z); }
		Point<num_t, Size> operator*=(const glm::tvec2<num_t>& pos) requires (Size == 2) {
			return Point<num_t, Size>(this->x = this->x * pos.x, this->y = this->y * pos.y); }
		Point<num_t, Size> operator/=(const glm::tvec4<num_t>& pos) requires (Size == 4) {
			return Point<num_t, Size>(this->x = this->x / pos.x, this->y = this->y / pos.y, this->z = this->z / pos.z, this->w = this->w / pos.w); }

		Point<num_t, Size> operator/=(const glm::tvec3<num_t>& pos) requires (Size == 3) {
			return Point<num_t, Size>(this->x = this->x / pos.x, this->y = this->y / pos.y, this->z = this->z / pos.z); }
		Point<num_t, Size> operator/=(const glm::tvec2<num_t>& pos) requires (Size == 2) {
			return Point<num_t, Size>(this->x = this->x / pos.x, this->y = this->y / pos.y); }
		Point<num_t, Size> operator%=(const glm::tvec4<num_t>& pos) requires (Size == 4) {
			return Point<num_t, Size>(this->x = this->x % pos.x, this->y = this->y % pos.y, this->z = this->z % pos.z, this->w = this->w % pos.w); }
		Point<num_t, Size> operator%=(const glm::tvec3<num_t>& pos) requires (Size == 3) {
			return Point<num_t, Size>(this->x = this->x % pos.x, this->y = this->y % pos.y, this->z = this->z % pos.z); }
		Point<num_t, Size> operator%=(const glm::tvec2<num_t>& pos) requires (Size == 2) {
			return Point<num_t, Size>(this->x = this->x % pos.x, this->y = this->y % pos.y); }
		Point<num_t, Size> operator&=(const glm::tvec4<num_t>& pos) requires (Size == 4) {
			return Point<num_t, Size>(this->x = this->x & pos.x, this->y = this->y & pos.y, this->z = this->z & pos.z, this->w = this->w & pos.w); }
		Point<num_t, Size> operator&=(const glm::tvec3<num_t>& pos) requires (Size == 3) {
			return Point<num_t, Size>(this->x = this->x & pos.x, this->y = this->y & pos.y, this->z = this->z & pos.z); }
		Point<num_t, Size> operator&=(const glm::tvec2<num_t>& pos) requires (Size == 2) {
			return Point<num_t, Size>(this->x = this->x & pos.x, this->y = this->y & pos.y); }

		Point<num_t, Size> operator^(const glm::tvec4<num_t>& pos) const requires (Size == 4) {
			return Point<num_t, Size>(num_t(pow(this->x, pos.x)), num_t(pow(this->y, pos.y)), num_t(pow(this->z, pos.z)), num_t(pow(this->w, pos.w))); }
		Point<num_t, Size> operator^(const glm::tvec3<num_t>& pos) const requires (Size == 3) {
			return Point<num_t, Size>(num_t(pow(this->x, pos.x)), num_t(pow(this->y, pos.y)), num_t(pow(this->z, pos.z))); }
		Point<num_t, Size> operator^(const glm::tvec2<num_t>& pos) const requires (Size == 2) {
			return Point<num_t, Size>(num_t(pow(this->x, pos.x)), num_t(pow(this->y, pos.y))); }
		Point<num_t, Size> operator+(const glm::tvec4<num_t>& pos) const requires (Size == 4) {
			return Point<num_t, Size>(this->x + pos.x, this->y + pos.y, this->z + pos.z, this->w + pos.w); }
		Point<num_t, Size> operator+(const glm::tvec3<num_t>& pos) const requires (Size == 3) {
			return Point<num_t, Size>(this->x + pos.x, this->y + pos.y, this->z + pos.z); }
		Point<num_t, Size> operator+(const glm::tvec2<num_t>& pos) const requires (Size == 2) {
			return Point<num_t, Size>(this->x + pos.x, this->y + pos.y); }
		Point<num_t, Size> operator-(const glm::tvec4<num_t>& pos) const requires (Size == 4) {
			return Point<num_t, Size>(this->x - pos.x, this->y - pos.y, this->z - pos.z, this->w - pos.w); }
		Point<num_t, Size> operator-(const glm::tvec3<num_t>& pos) const requires (Size == 3) {
			return Point<num_t, Size>(this->x - pos.x, this->y - pos.y, this->z - pos.z); }
		Point<num_t, Size> operator-(const glm::tvec2<num_t>& pos) const requires (Size == 2) {
			return Point<num_t, Size>(this->x - pos.x, this->y - pos.y); }
		Point<num_t, Size> operator*(const glm::tvec4<num_t>& pos) const requires (Size == 4) {
			return Point<num_t, Size>(this->x * pos.x, this->y * pos.y, this->z * pos.z, this->w * pos.w); }
		Point<num_t, Size> operator*(const glm::tvec3<num_t>& pos) const requires (Size == 3) {
			return Point<num_t, Size>(this->x * pos.x, this->y * pos.y, this->z * pos.z); }
		Point<num_t, Size> operator*(const glm::tvec2<num_t>& pos) const requires (Size == 2) {
			return Point<num_t, Size>(this->x * pos.x, this->y * pos.y); }
		Point<num_t, Size> operator/(const glm::tvec4<num_t>& pos) const requires (Size == 4) {
			return Point<num_t, Size>(this->x / pos.x, this->y / pos.y, this->z / pos.z, this->w / pos.w); }
		Point<num_t, Size> operator/(const glm::tvec3<num_t>& pos) const requires (Size == 3) {
			return Point<num_t, Size>(this->x / pos.x, this->y / pos.y, this->z / pos.z); }
		Point<num_t, Size> operator/(const glm::tvec2<num_t>& pos) const requires (Size == 2) {
			return Point<num_t, Size>(this->x / pos.x, this->y / pos.y); }
		Point<num_t, Size> operator%(const glm::tvec4<num_t>& pos) const requires (Size == 4) {
			return Point<num_t, Size>(this->x % pos.x, this->y % pos.y, this->z % pos.z, this->w % pos.w); }
		Point<num_t, Size> operator%(const glm::tvec3<num_t>& pos) const requires (Size == 3) {
			return Point<num_t, Size>(this->x % pos.x, this->y % pos.y, this->z % pos.z); }
		Point<num_t, Size> operator%(const glm::tvec2<num_t>& pos) const requires (Size == 2) {
			return Point<num_t, Size>(this->x % pos.x, this->y % pos.y); }
		Point<num_t, Size> operator&(const glm::tvec4<num_t>& pos) const requires (Size == 4) {
			return Point<num_t, Size>(this->x & pos.x, this->y & pos.y, this->z & pos.z, this->w & pos.w); }
		Point<num_t, Size> operator&(const glm::tvec3<num_t>& pos) const requires (Size == 3) {
			return Point<num_t, Size>(this->x & pos.x, this->y & pos.y, this->z & pos.z); }
		Point<num_t, Size> operator&(const glm::tvec2<num_t>& pos) const requires (Size == 2) {
			return Point<num_t, Size>(this->x & pos.x, this->y & pos.y); }

		operator glm::tvec4<num_t>() const requires (Size == 4) { return glm::tvec4<num_t>(this->x, this->y, this->z, this->w); }
		operator glm::tvec3<num_t>() const requires (Size == 3) { return glm::tvec3<num_t>(this->x, this->y, this->z); }
		operator glm::tvec2<num_t>() const requires (Size == 2) { return glm::tvec2<num_t>(this->x, this->y); }

		glm::tvec4<num_t> glm_cast() requires (Size == 4) { return this->operator glm::vec<4, num_t, glm::packed_highp>(); }
		glm::tvec3<num_t> glm_cast() requires (Size == 3) { return this->operator glm::vec<3, num_t, glm::packed_highp>(); }
		glm::tvec2<num_t> glm_cast() requires (Size == 2) { return this->operator glm::vec<2, num_t, glm::packed_highp>(); }

		static inline this_t random(num_t min=-1, num_t max=1) {
			this_t ret;
			for (std::size_t i=0; i < Size; i++) {
				std::random_device rd;
				std::mt19937 gen(rd());
				std::uniform_real_distribution<float> dist(min, max);
				ret.data[i] = dist(gen);
			}
			if constexpr (is_colored)  ret.color     = ret.color    ::random(min,max);
			if constexpr (has_noramls) ret.noramls   = ret.noramls  ::random(min,max);
			if constexpr (is_textured) ret.texcoords = ret.texcoords::random(min,max);
			return ret;
		}

		static inline this_t randomF(num_t min=-1, num_t max=1, int after_p = 1)
		requires (std::is_same_v<num_t, float> or std::is_same_v<num_t, double>) {
			int powered = (int)std::pow(10, after_p);
			this_cast_t<int> ret = this_cast_t<int>::random(min*powered, max*powered);
			return (this_t)(ret.template cast<float>()/powered);
		}



		inline void set_glpos() requires(Size == 2 or Size == 3) {
			if constexpr (Size == 3) {
				if constexpr (std::is_same_v<num_t, float>) glVertex3f(float(this->x), float(this->y), float(this->z));
				else if constexpr (std::is_same_v<num_t, double>) glVertex3d(double(this->x), double(this->y), double(this->z));
				else if constexpr (std::is_same_v<num_t, short>) glVertex3s(short(this->x), short(this->y), short(this->z));
				else if constexpr (std::is_same_v<num_t, int>) glVertex3i(int(this->x), int(this->y), int(this->z));
				else glVertex3f(float(this->x), float(this->y), float(this->z));
			} else if (Size == 2) {
				if constexpr (std::is_same_v<num_t, float>) glVertex2f(float(this->x), float(this->y));
				else if constexpr (std::is_same_v<num_t, double>) glVertex2d(double(this->x), double(this->y));
				else if constexpr (std::is_same_v<num_t, short>) glVertex2s(short(this->x), short(this->y));
				else if constexpr (std::is_same_v<num_t, int>) glVertex2i(int(this->x), int(this->y));
				else glVertex2f(float(this->x), float(this->y));
			}
		}

		inline void set_glcolor() requires(Size == 4) {
			if ((this->r * this->g * this->b * this->a) < 0) return;
			if constexpr (std::is_same_v<num_t, float>) {
				if (this->a == 1.0f) glColor3f(float(this->r), float(this->g), float(this->b));
				else glColor4f(float(this->r), float(this->g), float(this->b), float(this->a));
			} else if constexpr (std::is_same_v<num_t, double>) {
				if (this->a == 1.0) glColor3d(double(this->r), double(this->g), double(this->b));
				else glColor4d(double(this->r), double(this->g), double(this->b), double(this->a));
			} else if constexpr (std::is_same_v<num_t, short>) {
				if (this->a == 255) glColor3s(short(this->r), short(this->g), short(this->b));
				else glColor4s(short(this->r), short(this->g), short(this->b), short(this->a));
			} else if constexpr (std::is_same_v<num_t, int>) {
				if (this->a == 255) glColor3i(int(this->r), int(this->g), int(this->b));
				else glColor4i(int(this->r), int(this->g), int(this->b), int(this->a));
			} else if constexpr (std::is_same_v<num_t, uint>) {
				if (this->a == 255) glColor3ui(uint(this->r), uint(this->g), uint(this->b));
				else glColor4ui(uint(this->r), uint(this->g), uint(this->b), uint(this->a));
			} else if constexpr (std::is_same_v<num_t, GLbyte>) {
				if (this->a == GLbyte(255)) glColor3b(GLbyte(this->r), GLbyte(this->g), GLbyte(this->b));
				else glColor4b(GLbyte(this->r), GLbyte(this->g), GLbyte(this->b), GLbyte(this->a));
			} else glColor4f(float(this->r), float(this->g), float(this->b), float(this->a));
		}

		template<int setting = 0x0006, int algs_num = 360, int step = 1>
		void show(num_t rx = num_t(0.01f), num_t ry = num_t(0.01f), Point<num_t, 4> color = {-1,1,1,1}) requires(Size == 2 and !is_colored) {
			color.set_glcolor();
			glBegin(setting);
			for (std::size_t i = 0; i < algs_num; i += step) {
				float angle = float(i * 3.141592 / 180);
				(this->operator+({
					num_t(rx * std::cos(angle)),
					num_t(ry * std::sin(angle))
				})).set_glpos();
			}
			glEnd();
		}
		template<int setting = 0x0006, int algs_num = 360, int step = 1>
		void show(num_t rx = num_t(0.01f), num_t ry = num_t(0.01f)) requires(Size == 2 and is_colored) {
			this->color.set_glcolor();
			glBegin(setting);
			for (std::size_t i = 0; i < algs_num; i += step) {
				float angle = float(i * 3.141592 / 180);
				(this->operator+({
					num_t(rx * std::cos(angle)),
					num_t(ry * std::sin(angle))
				})).set_glpos();
			}
			glEnd();
		}

		template<int setting = 0x0006, int algs_num = 360, int step = 1>
		void show(num_t rx, num_t ry, std::vector<Point<num_t, 4>> colors) requires(Size == 2) {
			glBegin(setting);
				for (std::size_t i = 0, size = colors.size(); i < algs_num; i += step) {
					float angle = float(i * 3.141592 / 180);
					uint colors_i = uint(i / step);
					if (i < size) colors[colors_i].set_glcolor();
					(this->operator+({
						num_t(rx * std::cos(angle)),
						num_t(ry * std::sin(angle))
					})).set_glpos();
				}
			glEnd();
		}



		template<int setting = 0x0006, int algs_num = 360, int step = 1>
		void show(num_t rx = num_t(0.01f), num_t ry = num_t(0.01f), num_t rz = num_t(0.01f), Point<num_t, 4> color = {-1,1,1,1}) requires(Size == 3 and !is_colored) {
			color.set_glcolor();
			glBegin(setting);
			for (std::size_t i = 0; i < algs_num; i += step) {
				float angle = float(i * 3.141592 / 180);
				(this->operator+({
					num_t(rx * std::cos(angle)),
					num_t(ry * std::sin(angle)),
					rz * angle
				})).set_glpos();
			}
			glEnd();
		}
		template<int setting = 0x0006, int algs_num = 360, int step = 1>
		void show(num_t rx = num_t(0.01f), num_t ry = num_t(0.01f), num_t rz = num_t(0.01f)) requires(Size == 3 and is_colored) {
			this->color.set_glcolor();
			glBegin(setting);
			for (std::size_t i = 0; i < algs_num; i += step) {
				float angle = float(i * 3.141592 / 180);
				(this->operator+({
					num_t(rx * std::cos(angle)),
					num_t(ry * std::sin(angle)),
					rz * angle
				})).set_glpos();
			}
			glEnd();
		}

		template<int setting = 0x0006, int algs_num = 360, int step = 1>
		void show(num_t rx, num_t ry, num_t rz, std::vector<Point<num_t, 4>> colors) requires(Size == 3) {
			glBegin(setting);
				for (std::size_t i = 0, size = colors.size(); i < algs_num; i += step) {
					float angle = float(i * 3.141592 / 180);
					uint colors_i = uint(i / step);
					if (i < size) colors[colors_i].set_glcolor();
					(this->operator+({
						num_t(rx * std::cos(angle)),
						num_t(ry * std::sin(angle)),
						rz * angle
					})).set_glpos();
				}
			glEnd();
		}
	};



	template<typename num_t> using Point2D =   Point<num_t, 2, false, false, false>;
	template<typename num_t> using Point3D =   Point<num_t, 3, false, false, false>;
	template<typename num_t> using Point4D =   Point<num_t, 4, false, false, false>;
	template<typename num_t> using VertexPT2D = Point<num_t, 2, false, false, true>;
	template<typename num_t> using VertexPT3D = Point<num_t, 3, false, false, true>;
	template<typename num_t> using VertexPN2D = Point<num_t, 2, false, true, false>;
	template<typename num_t> using VertexPN3D = Point<num_t, 3, false, true, false>;
	template<typename num_t> using CPoint2D   = Point<num_t, 2, true, false, false>;
	template<typename num_t> using CPoint3D   = Point<num_t, 3, true, false, false>;
	template<typename num_t> using VertexPNT2D = Point<num_t, 2, false, true, true>;
	template<typename num_t> using VertexPNT3D = Point<num_t, 3, false, true, true>;
	template<typename num_t> using VertexPCN2D = Point<num_t, 2, true, true, false>;
	template<typename num_t> using VertexPCN3D = Point<num_t, 3, true, true, false>;
	template<typename num_t> using VertexPCT2D = Point<num_t, 2, true, false, true>;
	template<typename num_t> using VertexPCT3D = Point<num_t, 3, true, false, true>;
	template<typename num_t> using Vertex2D = Point<num_t, 2, true, true, true>;
	template<typename num_t> using Vertex3D = Point<num_t, 3, true, true, true>;
	using PointF2D =   Point<float, 2, false, false, false>;
	using PointF3D =   Point<float, 3, false, false, false>;
	using PointF4D =   Point<float, 4, false, false, false>;
	using VertexFPT2D = Point<float, 2, false, false, true>;
	using VertexFPT3D = Point<float, 3, false, false, true>;
	using VertexFPN2D = Point<float, 2, false, true, false>;
	using VertexFPN3D = Point<float, 3, false, true, false>;
	using CPointF2D =   Point<float, 2, true, false, false>;
	using CPointF3D =   Point<float, 3, true, false, false>;
	using VertexFPNT2D = Point<float, 2, false, true, true>;
	using VertexFPNT3D = Point<float, 3, false, true, true>;
	using VertexFPCN2D = Point<float, 2, true, true, false>;
	using VertexFPCN3D = Point<float, 3, true, true, false>;
	using VertexFPCT2D = Point<float, 2, true, false, true>;
	using VertexFPCT3D = Point<float, 3, true, false, true>;
	using VertexF2D = Point<float, 2, true, true, true>;
	using VertexF3D = Point<float, 3, true, true, true>;



	// custemed @c 2d Rectangle
	template<typename num_t> class Rect<num_t, 2> {
	public :
		using point_t = Point<num_t, 2>;
		union {
			// use as Rectangle union
			struct { num_t x , y , width, height; };
			// use as Line (with points[2] : a, b) union
			struct { num_t xa, ya, xb   , yb	; };
			// use as dynamice array union
			struct { point_t posA, posB; };
			struct { point_t start, end; };
			num_t data[4];
		};
		// constracters
		Rect(num_t x = 0, num_t y = 0, num_t w = 0, num_t h = 0)
			:x(x), y(y), width(w), height(h) {}
		Rect(num_t data[4])
			:data(data) {}

		// get elements operation
		num_t operator [] (std::size_t i) { return data[i]; }

		// setting and getting
		sget_addition( num_t, height )
		sget_addition( num_t, width  )
		sget_addition( num_t, x	  )
		sget_addition( num_t, y	  )
	};

	// custemed @c 3d Rectangle
	template<typename num_t> class Rect<num_t, 3> {
	public :
		using point_t = Point<num_t, 3>;
		union {
			// use as Rectangle union
			struct { num_t x ,  y ,  z ,  width,  height,  length; };
			// use as line union
			struct { num_t xa,  ya,  za,  xb   ,  yb	,  zb	; };
			// use as dynamice array union
			struct { point_t posA, posB; };
			struct { point_t start, end; };
			num_t data[6];
		};
		// constracter
		Rect(num_t x = 0, num_t y = 0, num_t z = 0, num_t w = 0, num_t h = 0, num_t l = 0)
			:x(x), y(y), z(z), width(w), height(h), length(l) {}
		Rect(num_t data[6])
			:data(data) {}

		// get elements operation
		num_t operator [] (std::size_t i) { return data[i]; }
		sget_addition( num_t, length )
		sget_addition( num_t, height )
		sget_addition( num_t, width  )
		sget_addition( num_t, x	  )
		sget_addition( num_t, y	  )
		sget_addition( num_t, z	  )
	};

	// custemed @c 4d Rectangle
	template<typename num_t> class Rect<num_t, 4> {
	public :
		using point_t = Point<num_t, 4>;
		union {
			// use as Rectangle union
			struct { num_t x , y , z , w , width, height, length, power; };
			// use as line union
			struct { num_t xa, ya, za, wa, xb   , yb	, zb	, wb   ; };
			// use as dynamice array union
			struct { point_t posA, posB; };
			struct { point_t start, end; };
			num_t data[8];
		};
		// constracters
		Rect(num_t x = 0, num_t y = 0, num_t z = 0, num_t w = 0, num_t wd = 0, num_t h = 0, num_t l = 0, num_t p = 0)
			:x(x), y(y), z(z), w(w), width(wd), height(h), length(l), power(p) {}
		Rect(num_t data[8])
			:data(data) {}

		// get elements operations
		num_t operator [] (std::size_t i) { return data[i]; }

		// setting and getting
		sget_addition( num_t, length )
		sget_addition( num_t, height )
		sget_addition( num_t, width  )
		sget_addition( num_t, power  )
		sget_addition( num_t, x	  )
		sget_addition( num_t, y	  )
		sget_addition( num_t, z	  )
		sget_addition( num_t, w	  )
	};



	template<typename num_t, std::size_t Size> class PointNoramls_extention<num_t, Size, true> { 
	public : union {
			Point<num_t, Size> noramls;
			num_t n_data[Size];
		}; PointNoramls_extention(num_t n_data[Size]) : n_data(n_data) {}
		PointNoramls_extention() {}
	};
	template<typename num_t> class PointTexcoords_extention<num_t, true> {
	public : union {
			Point2D<num_t> texcoords;
			num_t txc_data[2];
		}; PointTexcoords_extention(num_t txc_data[2]) : txc_data(txc_data) {}
		PointTexcoords_extention() {}
	};
	template<typename num_t> class PointColor_extention<num_t, true> {
	public : union {
			Point4D<num_t> color;
			num_t c_data[4];
		}; PointColor_extention(num_t c_data[4]) : c_data(c_data) {}
		PointColor_extention() {}
	};
}

tuple_addition_p2(geometry::Point2D<double>, 2, double)
tuple_addition_p2(geometry::Point3D<double>, 3, double)
tuple_addition_p2(geometry::Point4D<double>, 3, double)
tuple_addition_p2(geometry::Point2D<float>,  2, float )
tuple_addition_p2(geometry::Point3D<float>,  3, float )
tuple_addition_p2(geometry::Point4D<float>,  3, float )
tuple_addition_p2(geometry::Point2D<long>,   2, long  )
tuple_addition_p2(geometry::Point3D<long>,   3, long  )
tuple_addition_p2(geometry::Point4D<long>,   3, long  )
tuple_addition_p2(geometry::Point2D<int>,    2, int   )
tuple_addition_p2(geometry::Point3D<int>,    3, int   )
tuple_addition_p2(geometry::Point4D<int>,    3, int   )
