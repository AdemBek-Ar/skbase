module;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>
#include <GL/glu.h>

export module skbase.geomath.line;

import skbase.json;
import skbase.base;
import skbase.memory.array;
import skbase.geomath.math;
import std;

#include "../defines.h++"

import skbase.geomath.point;

namespace file {
	extern std::ofstream  save        (const char*    filename, const char*    src);
	extern std::string    get_contents(const char*    filename                    );
}

#include "../extentions/str/wstrbase.h++"
#include "../extentions/str/strbase.h++"



export namespace geometry {
	template<
		typename num_t,
		std::size_t Size,
		bool is_colored = false,
		bool has_noramls = false,
		bool is_textured = false>
	requires (Size > 0 && std::is_arithmetic_v<num_t>)
	class Line : public Rect<num_t, Size> {
	public :
		using point_t = Point<num_t, Size, is_colored, has_noramls, is_textured>;
		using Rect<num_t, Size>::posA; using Rect<num_t, Size>::start;
		using Rect<num_t, Size>::posB; using Rect<num_t, Size>::end;
		using Rect<num_t, Size>::data;
		Line(num_t xa, num_t ya, num_t za, num_t xb, num_t yb, num_t zb)
			requires (Size == 3) : Rect<num_t, Size>{xa, ya, za, xb, yb, zb} {}
		Line(num_t xa, num_t ya, num_t xb, num_t yb) requires (Size == 2)
			: Rect<num_t, Size>({xa, ya, xb, yb}) {}
		Line(point_t posA = {}, point_t posB = {})
			: Rect<num_t, Size>(posA, posB) {}

		std::string tostring(std::string spliter=",")
			{ return posA.tostring(spliter) + spliter + posB.tostring(spliter); }
		void fromstring(std::string str, std::string spliter=",") {
			std::vector<std::string> strs = str::split(str, spliter);
			posA.fromstring(strs[0]);
			posB.fromstring(strs[1]);
		}
		std::vector<byte> tobytes(std::string spliter=",") {
			std::vector<byte> ret;
			std::string th_str = tostring();
			for(char ch : th_str) ret.push_back(static_cast<byte>(ch));
			return ret;
		}
		void frombytes(std::vector<byte> bytes, std::string spliter=",") {
			std::string reading_from="";
			for(byte b : bytes) reading_from += static_cast<char>(b);
			fromstring(reading_from);
		}
		void tofile(std::string path, std::string spliter=",")
			{ file::save(path.c_str(), tostring().c_str()); }
		void fromfile(std::string path, std::string spliter=",")
			{ fromstring(file::get_contents(path.c_str()), spliter); }
		void print() { outn(tostring()) }

		std::wstring towstring(std::wstring spliter=L",") { return posA.towstring(spliter) + spliter + posB.towstring(spliter); }
		void fromwstring(std::wstring str, std::wstring spliter=L",") {
			std::vector<std::wstring> strs = str::split(str, spliter);
			posA.fromwstring(strs[0]);
			posB.fromwstring(strs[1]);
		}
		void wprint() { woutn(towstring()) }

		json tojson() {
			json j;
			j["A"] = posA.tojson();
			j["B"] = posB.tojson();
			return j;
		}
		void fromjson(json j) {
			posA.fromjson(j["A"]);
			posB.fromjson(j["B"]);
		}
		std::string tojsonstr() { return tojson().dump(4); }
		void fromjsonstr(std::string str) {
			json j = json::parse(str);
			fromjson(j);
		}
		std::ofstream tojsonfile(std::string path)
			{ return file::save(path.c_str(), tojsonstr().c_str()); }
		void fromjsonfile(std::string path)
			{ fromjsonstr(file::get_contents(path.c_str())); }

		Line operator^=(const Line<num_t, Size>& line) { return Line<num_t, Size>(posA ^= line.posA, posB ^= line.posB); }
		Line operator^=(const point_t& pos) { return Line<num_t, Size>(posA ^= pos, posB ^= pos); }
		Line operator^=(const num_t& var) { return Line<num_t, Size>(posA ^= var, posB ^= var); }
		Line operator+=(const Line<num_t, Size>& line) { return Line<num_t, Size>(posA += line.posA, posB += line.posB); }
		Line operator+=(const point_t& pos) { return Line<num_t, Size>(posA += pos, posB += pos); }
		Line operator+=(const num_t& var) { return Line<num_t, Size>(posA += var, posB += var); }
		Line operator-=(const Line<num_t, Size>& line) { return Line<num_t, Size>(posA -= line.posA, posB -= line.posB); }
		Line operator-=(const point_t& pos) { return Line<num_t, Size>(posA -= pos, posB -= pos); }
		Line operator-=(const num_t& var) { return Line<num_t, Size>(posA -= var, posB -= var); }
		Line operator*=(const Line<num_t, Size>& line) { return Line<num_t, Size>(posA *= line.posA, posB *= line.posB); }
		Line operator*=(const point_t& pos) { return Line<num_t, Size>(posA *= pos, posB *= pos); }
		Line operator*=(const num_t& var) { return Line<num_t, Size>(posA *= var, posB *= var); }
		Line operator/=(const Line<num_t, Size>& line) { return Line<num_t, Size>(posA /= line.posA, posB /= line.posB); }
		Line operator/=(const point_t& pos) { return Line<num_t, Size>(posA /= pos, posB /= pos); }
		Line operator/=(const num_t& var) { return Line<num_t, Size>(posA /= var, posB /= var); }
		Line operator%=(const Line<num_t, Size>& line) { return Line<num_t, Size>(posA %= line.posA, posB %= line.posB); }
		Line operator%=(const point_t& pos) { return Line<num_t, Size>(posA %= pos, posB %= pos); }
		Line operator%=(const num_t& var) { return Line<num_t, Size>(posA %= var, posB %= var); }
		Line operator&=(const Line<num_t, Size>& line) { return Line<num_t, Size>(posA &= line.posA, posB &= line.posB); }
		Line operator&=(const point_t& pos) { return Line<num_t, Size>(posA &= pos, posB &= pos); }
		Line operator&=(const num_t& var) { return Line<num_t, Size>(posA &= var, posB &= var); }

		Line operator^(const Line<num_t, Size>& line) { return Line<num_t, Size>(posA ^ line.posA, posB ^ line.posB); }
		Line operator^(const point_t& pos) { return Line<num_t, Size>(posA ^ pos, posB ^ pos); }
		Line operator^(const num_t& var) { return Line<num_t, Size>(posA ^ var, posB ^ var); }
		Line operator+(const Line<num_t, Size>& line) { return Line<num_t, Size>(posA + line.posA, posB + line.posB); }
		Line operator+(const point_t& pos) { return Line<num_t, Size>(posA + pos, posB + pos); }
		Line operator+(const num_t& var) { return Line<num_t, Size>(posA + var, posB + var); }
		Line operator-(const Line<num_t, Size>& line) { return Line<num_t, Size>(posA - line.posA, posB - line.posB); }
		Line operator-(const point_t& pos) { return Line<num_t, Size>(posA - pos, posB - pos); }
		Line operator-(const num_t& var) { return Line<num_t, Size>(posA - var, posB - var); }
		Line operator*(const Line<num_t, Size>& line) { return Line<num_t, Size>(posA * line.posA, posB * line.posB); }
		Line operator*(const point_t& pos) { return Line<num_t, Size>(posA * pos, posB * pos); }
		Line operator*(const num_t& var) { return Line<num_t, Size>(posA * var, posB * var); }
		Line operator/(const Line<num_t, Size>& line) { return Line<num_t, Size>(posA / line.posA, posB / line.posB); }
		Line operator/(const point_t& pos) { return Line<num_t, Size>(posA / pos, posB / pos); }
		Line operator/(const num_t& var) { return Line<num_t, Size>(posA / var, posB / var); }
		Line operator%(const Line<num_t, Size>& line) { return Line<num_t, Size>(posA % line.posA, posB % line.posB); }
		Line operator%(const point_t& pos) { return Line<num_t, Size>(posA % pos, posB % pos); }
		Line operator%(const num_t& var) { return Line<num_t, Size>(posA % var, posB % var); }
		Line operator&(const Line<num_t, Size>& line) { return Line<num_t, Size>(posA & line.posA, posB & line.posB); }
		Line operator&(const point_t& pos) { return Line<num_t, Size>(posA & pos, posB & pos); }
		Line operator&(const num_t& var) { return Line<num_t, Size>(posA & var, posB & var); }

		Line operator^=(const glm::tvec2<num_t>& pos) requires(Size >= 2 and Size < 5) { return Line<num_t, Size>(posA ^= pos, posB ^= pos); }
		Line operator+=(const glm::tvec2<num_t>& pos) requires(Size >= 1 and Size < 5) { return Line<num_t, Size>(posA += pos, posB += pos); }
		Line operator-=(const glm::tvec2<num_t>& pos) requires(Size >= 1 and Size < 5) { return Line<num_t, Size>(posA -= pos, posB -= pos); }
		Line operator*=(const glm::tvec2<num_t>& pos) requires(Size >= 1 and Size < 5) { return Line<num_t, Size>(posA *= pos, posB *= pos); }
		Line operator/=(const glm::tvec2<num_t>& pos) requires(Size >= 1 and Size < 5) { return Line<num_t, Size>(posA /= pos, posB /= pos); }
		Line operator%=(const glm::tvec2<num_t>& pos) requires(Size >= 1 and Size < 5) { return Line<num_t, Size>(posA %= pos, posB %= pos); }
		Line operator&=(const glm::tvec2<num_t>& pos) requires(Size >= 1 and Size < 5) { return Line<num_t, Size>(posA &= pos, posB &= pos); }

		Line operator^(const glm::tvec2<num_t>& pos) requires(Size >= 2 and Size < 5) { return Line<num_t, Size>(posA ^ pos, posB ^ pos); }
		Line operator+(const glm::tvec2<num_t>& pos) requires(Size >= 1 and Size < 5) { return Line<num_t, Size>(posA + pos, posB + pos); }
		Line operator-(const glm::tvec2<num_t>& pos) requires(Size >= 1 and Size < 5) { return Line<num_t, Size>(posA - pos, posB - pos); }
		Line operator*(const glm::tvec2<num_t>& pos) requires(Size >= 1 and Size < 5) { return Line<num_t, Size>(posA * pos, posB * pos); }
		Line operator/(const glm::tvec2<num_t>& pos) requires(Size >= 1 and Size < 5) { return Line<num_t, Size>(posA / pos, posB / pos); }
		Line operator%(const glm::tvec2<num_t>& pos) requires(Size >= 1 and Size < 5) { return Line<num_t, Size>(posA % pos, posB % pos); }
		Line operator&(const glm::tvec2<num_t>& pos) requires(Size >= 1 and Size < 5) { return Line<num_t, Size>(posA & pos, posB & pos); }

		operator glm::tvec4<num_t>() const requires (Size == 4) { point_t d = posB - posA; return glm::tvec4<num_t>(d.x, d.y, d.z, d.w); }

		operator glm::tvec3<num_t>() const requires (Size == 3) { point_t d = posB - posA; return glm::tvec4<num_t>(d.x, d.y, d.z); }
		operator glm::tvec2<num_t>() const requires (Size == 2) { point_t d = posB - posA; return glm::tvec4<num_t>(d.x, d.y); }

		glm::tvec4<num_t>* glm_cast() requires (Size == 4) { return new point_t[2]{ posA.glm_cast(), posB.glm_cast() }; }
		glm::tvec3<num_t>* glm_cast() requires (Size == 3) { return new point_t[2]{ posA.glm_cast(), posB.glm_cast() }; }
		glm::tvec2<num_t>* glm_cast() requires (Size == 2) { return new point_t[2]{ posA.glm_cast(), posB.glm_cast() }; }

		num_t length() { return num_t((posB - posA).length()); }

		point_t cross() requires (Size > 2) { return this->start.cross(this->end); }
		num_t cross() requires (Size == 2) { return this->start.cross(this->end); }
		point_t dot() requires (Size > 2) { return this->start.dot(this->end); }
		num_t dot() requires (Size == 2) { return this->start.dot(this->end); }

		point_t get_slope() requires (Size >= 3) { return this->posB - this->posA; }
		num_t get_slope() requires (Size == 2) {
			point_t d = this->end - this->start;
			if (d.x == 0) return INFINITY;
			else return d.y / d.x;
		}

		bool is_parallel(const Line<num_t, Size>& other) requires (Size == 2) {
			point_t th_d = end - start, oth_d = other.end - other.start;
			if (th_d.length() < (1e-7) or oth_d.length() < (1e-7)) return false;
			num_t cross = th_d.cross(oth_d);
			return fabs(cross) < (1e-7);
		}
		bool is_parallel(const Line<num_t, Size>& other) requires (Size == 3) {
			point_t th_d = end - start, oth_d = other.end - other.start;
			if (th_d.length() < (1e-7) or oth_d.length() < (1e-7)) return false;
			point_t cross = th_d.cross(oth_d);
			return cross.length() < (1e-7);
		}
		bool is_parallel(const Line<num_t, Size>& other) requires (Size > 3) {
			point_t th_d = end - start, oth_d = other.end - other.start;
			if (th_d.length() < (1e-7) or oth_d.length() < (1e-7)) return false;
			num_t ratio = num_t(0.0);
			bool ratio_set = false;
			for (int i = 0; i < int(Size); i++) if (fabs(th_d.data[i]) > (1e-7) and fabs(oth_d.data[i]) > (1e-7)) {
				if (i == 0) ratio = oth_d.data[i] / th_d.data[i];
				else {
					num_t r = oth_d.data[i] / th_d.data[i];
					if (ratio_set and fabs(r - ratio) > (1e-7)) return false;
					ratio = r;
				} ratio_set = true;
			}
		}

		bool is_perpendicular(const Line<num_t, Size>& other) requires (Size == 2) {
			point_t th_d = end - start, oth_d = other.end - other.start;
			if (th_d.length() < (1e-7) or oth_d.length() < (1e-7)) return false;
			num_t dot = th_d.dot(oth_d);
			return fabs(dot) < (1e-7);
		}
		bool is_perpendicular(const Line<num_t, Size>& other) requires (Size > 2) {
			point_t th_d = end - start, oth_d = other.end - other.start;
			if (th_d.length() < (1e-7) or oth_d.length() < (1e-7)) return false;
			point_t dot = th_d.dot(oth_d);
			return dot.length() < (1e-7);
		}

		num_t get_angle() const requires (Size == 2) {
			point_t d =this->end - this->start ;
			return num_t(atan2(d.x, d.y) * 180 / M_PI);
		}
		std::array<num_t, Size> get_angle() const requires (Size > 2) {
			point_t d = this->end - this->start;
			num_t len = d.length();
			if (len < (1e-7)) throw std::invalid_argument("can't calc for a Zero line");
			std::array<num_t, Size> ret;
			for (int i = 0; i < int(Size); i++) ret[i] = num_t(acos(d.data[i] / len) * 180 / M_PI);
			return ret;
		}

		num_t angle_to(const Line<num_t, Size>& line) const {
			point_t d1 = this->end - this->start, d2 = line.end - line.start;
			return d1.angle_to(d2);
		}
		num_t angle_to(const Point<num_t, Size>& point) const {
			point_t d1 = this->end - this->start, d2 = point - this->start;
			return d1.angle_to(d2);
		}

		Arrays::Array<Line<num_t, Size>> split(const uint& number) {
			point_t len = posA - posB;
			Arrays::Array<Line<num_t, Size>> ret = Arrays::Array<Line<num_t, Size>>();
			for (int i = 0; i < number; i++) {
				point_t start = posA + (len * i);
				ret += Line<num_t, Size>(start, start + len);
			} return ret;
		}

		inline bool is_touched(const point_t& posA, const point_t& posB) { return this->is_touched({ posA, posB }); }

		bool is_touched(const Line<num_t, Size>& line) requires (Size == 2) {
			point_t p1 = this->start, p2 = this->end, p3 = line.start, p4 = line.end;
			num_t denom = (p4.y - p3.y) * (p2.x - p1.x) - (p4.x - p3.x) * (p2.y - p1.y);
			if (fabs(denom) < (1e-7)) return false;
			num_t ua = ((p4.x - p3.x) * (p1.y - p3.y) - (p4.y - p3.y) * (p1.x - p3.x)) / denom,
				  ub = ((p2.x - p1.x) * (p1.y - p3.y) - (p2.y - p1.y) * (p1.x - p3.x)) / denom;
			return ua >= 0 and ua <= 1 and ub >= 0 and ub <= 1;
		}
		bool is_touched(const Line<num_t, Size>& line) requires (Size == 3) {
			point_t d1 = this->end - this->start, d2 = line.end - line.start, b = line.start - this->start;
			num_t d1d1 = d1.dot(d1), d1d2 = d1.dot(d2), d2d2 = d2.dot(d2), d1b = d1.dot(b), d2b = d2.dot(b);
			num_t det = d1d1 * d2d2 - d1d2 * d1d2;
			if (fabs(det) < (1e-7)) return false;
			num_t t = (d1b * d2d2 - d1d2 * d2b) / det,
				  s = (d1b * d1d2 - d1d1 * d2b) / det;
			return t >= 0 and t <= 1 and s >= 0 and s <= 1;
		}

		point_t get_center() {
			point_t ret = posA + posB;
			return point_t(ret.x / 2, ret.y / 2);
		}

		num_t distance_to(const point_t& other) { return this->get_center().distance_to(other); }

		Line<num_t, Size> rot(num_t x, num_t y, num_t z, num_t AlgX, num_t AlgY, num_t AlgZ, bool rot_this = 1) requires (Size == 3) { return Line<num_t, Size>(posA.rot(x, y, z, AlgX, AlgY, AlgZ, rot_this), posB.rot(x, y, z, AlgX, AlgY, AlgZ, rot_this)); }
		Line<num_t, Size> rotX(num_t x, num_t y, num_t z, num_t Alg, bool rot_this = 1) requires (Size == 3) { return Line<num_t, Size>(posA.rotX(x, y, z, Alg, rot_this), posB.rotX(x, y, z, Alg, rot_this)); }
		Line<num_t, Size> rotY(num_t x, num_t y, num_t z, num_t Alg, bool rot_this = 1) requires (Size == 3) { return Line<num_t, Size>(posA.rotY(x, y, z, Alg, rot_this), posB.rotY(x, y, z, Alg, rot_this)); }
		Line<num_t, Size> rotZ(num_t x, num_t y, num_t z, num_t Alg, bool rot_this = 1) requires (Size == 3) { return Line<num_t, Size>(posA.rotZ(x, y, z, Alg, rot_this), posB.rotZ(x, y, z, Alg, rot_this)); }
		Line<num_t, Size> rot(num_t x, num_t y, num_t z, num_t Alg, bool rot_this = 1) requires (Size == 3) { return Line<num_t, Size>(posA.rot(x, y, z, Alg, rot_this), posB.rot(x, y, z, Alg, rot_this)); }
		Line<num_t, Size> rot(num_t x, num_t y, num_t Alg, bool rot_this = 1) requires (Size == 2) { return Line<num_t, Size>(posA.rot(x, y, Alg, rot_this), posB.rot(x, y, Alg, rot_this)); }
		inline Line<num_t, Size> rot(const point_t& pos, num_t Alg) requires (Size == 2 or Size == 3) { return (Size == 2) ? rot(pos.x, pos.y, Alg) : rot(pos.x, pos.y, pos.z, Alg); }
		inline Line<num_t, Size> rot(const point_t& pos, num_t AlgX, num_t AlgY, num_t AlgZ) requires (Size == 3) { return rot(pos.x, pos.y, pos.z, AlgX, AlgY, AlgZ); }

		template<int setting = 0x0006, int algs_num = 360, int step = 1>
		inline void show_points(num_t rx = num_t(0.01f), num_t ry = num_t(0.01f), Point<num_t, 4> color = {-1,1,1,1}) requires(Size == 2 and !is_colored) {
			posA.template show<setting, algs_num, step>(rx, ry, color);
			posB.template show<setting, algs_num, step>(rx, ry, color);
		}
		template<int setting = 0x0006, int algs_num = 360, int step = 1>
		inline void show_points(num_t rx = num_t(0.01f), num_t ry = num_t(0.01f)) requires(Size == 2 and is_colored) {
			posA.template show<setting, algs_num, step>(rx, ry);
			posB.template show<setting, algs_num, step>(rx, ry);
		}
		template<int setting = 0x0006, int algs_num = 360, int step = 1>
		inline void show_points(num_t rx, num_t ry, std::vector<Point<num_t, 4>> colors) requires(Size == 2) {
			posA.template show<setting, algs_num, step>(rx, ry, colors);
			posB.template show<setting, algs_num, step>(rx, ry, colors);
		}
		
		template<int setting = GL_LINE_LOOP, float line_size = -1>
		inline void show(Point<num_t, 4> color_a = {-1,1,1,1}, Point<num_t, 4> color_b = {-1,1,1,1}) requires(Size == 2 and !is_colored) {
			if constexpr (line_size > 0) glLineWidth(line_size);
			glBegin(setting);
				color_a.set_glcolor();
				posA.set_glpos();
				color_b.set_glcolor();
				posB.set_glpos();
			glEnd();
		}
		template<int setting = GL_LINE_LOOP, float line_size = -1>
		inline void show() requires(Size == 2 and is_colored) {
			if constexpr (line_size > 0) glLineWidth(line_size);
			glBegin(setting);
				posA.color.set_glcolor();
				posA.color.set_glpos();
				posB.color.set_glcolor();
				posB.color.set_glpos();
			glEnd();
		}
	};



	template<typename num_t> using Line2D =   Line<num_t, 2, false, false, false>;
	template<typename num_t> using Line3D =   Line<num_t, 3, false, false, false>;
	template<typename num_t> using Line4D =   Line<num_t, 4, false, false, false>;
	template<typename num_t> using LineT2D = Line<num_t, 2, false, false, true>;
	template<typename num_t> using LineT3D = Line<num_t, 3, false, false, true>;
	template<typename num_t> using LineN2D = Line<num_t, 2, false, true, false>;
	template<typename num_t> using LineN3D = Line<num_t, 3, false, true, false>;
	template<typename num_t> using LineC2D = Line<num_t, 2, true, false, false>;
	template<typename num_t> using LineC3D = Line<num_t, 3, true, false, false>;
	template<typename num_t> using LineNT2D = Line<num_t, 2, false, true, true>;
	template<typename num_t> using LineNT3D = Line<num_t, 3, false, true, true>;
	template<typename num_t> using LineCN2D = Line<num_t, 2, true, true, false>;
	template<typename num_t> using LineCN3D = Line<num_t, 3, true, true, false>;
	template<typename num_t> using LineCT2D = Line<num_t, 2, true, false, true>;
	template<typename num_t> using LineCT3D = Line<num_t, 3, true, false, true>;
	template<typename num_t> using FLine2D = Line<num_t, 2, true, true, true>;
	template<typename num_t> using FLine3D = Line<num_t, 3, true, true, true>;
	using LineF2D =   Line<float, 2, false, false, false>;
	using LineF3D =   Line<float, 3, false, false, false>;
	using LineF4D =   Line<float, 4, false, false, false>;
	using LineFT2D = Line<float, 2, false, false, true>;
	using LineFT3D = Line<float, 3, false, false, true>;
	using LineFN2D = Line<float, 2, false, true, false>;
	using LineFN3D = Line<float, 3, false, true, false>;
	using LineFC2D = Line<float, 2, true, false, false>;
	using LineFC3D = Line<float, 3, true, false, false>;
	using LineFNT2D = Line<float, 2, false, true, true>;
	using LineFNT3D = Line<float, 3, false, true, true>;
	using LineFCN2D = Line<float, 2, true, true, false>;
	using LineFCN3D = Line<float, 3, true, true, false>;
	using LineFCT2D = Line<float, 2, true, false, true>;
	using LineFCT3D = Line<float, 3, true, false, true>;
	using FLineF2D = Line<float, 2, true, true, true>;
	using FLineF3D = Line<float, 3, true, true, true>;
}
