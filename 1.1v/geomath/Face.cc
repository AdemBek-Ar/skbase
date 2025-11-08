module;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>
#include <GL/glu.h>

export module skbase.geomath.face;

import skbase.json;
import skbase.base;
import skbase.memory.array;
import skbase.geomath.math;
import std;

#include "../defines.h++"

#define TXT_TO_NUM(str) \
math::Number::to_num

import skbase.geomath.line;
import skbase.geomath.point;

namespace file {
	extern std::ofstream  save        (const char*    filename, const char*    src);
	extern std::string    get_contents(const char*    filename                    );
}

#include "../extentions/str/wstrbase.h++"
#include "../extentions/str/strbase.h++"



export namespace geometry {
	template<bool Enable = false> class FaceIndices_extention;
	template<> class FaceIndices_extention<false> {};
	template<> class FaceIndices_extention<true> { 
		public : Arrays::Array<uint> indices;
		FaceIndices_extention(std::vector<uint> indices) :indices(indices) {}
		FaceIndices_extention() {} 
	};

	template<
		typename num_t,
		std::size_t Size,
		bool is_colored = false,
		bool has_noramls = false,
		bool is_textured = false,
		bool has_indices = false>
	requires (Size > 0 && std::is_arithmetic_v<num_t>)
	class Face : public FaceIndices_extention<has_indices> {
	public : using this_t = Face <num_t, Size, is_colored, has_noramls, is_textured, has_indices>;
		using line_t = Line<num_t, Size, is_colored, has_noramls, is_textured>;
		using point_t= line_t::point_t;
		union {
			Arrays::Array<point_t> points ;
			Arrays::Array<num_t  > vertexs;
		};

		Face(std::initializer_list<num_t> init, std::vector<uint> indices) requires(has_indices) : vertexs(init   ) { this->indices = indices; }
		Face(std::vector<point_t> points, std::vector<uint> indices	  ) requires(has_indices) : points (points ) { this->indices = indices; }
		Face(std::vector<num_t> vertexs, std::vector<uint> indices	   ) requires(has_indices) : vertexs(vertexs) { this->indices = indices; }

		Face(std::initializer_list<num_t> init) : vertexs(init   ) { }
		Face(std::vector<point_t> points	  ) : points (points ) { }
		Face(std::vector<num_t> vertexs	   ) : vertexs(vertexs) { }

		Face () {}
		~Face() {}

		point_t operator [] (const std::size_t& i) { return this->points[i]; }

		std::size_t length() { return this->points.size(); }

		Face operator^=(const point_t& pos) { for (point_t& point : points) point ^= pos; return *this; }
		Face operator^=(const num_t& var) { for (point_t& point : points) point ^= var; return *this; }
		Face operator^=(const Face<num_t, Size>& face) {
			for (int i = 0, length = int((this->length() < face.length()) ? this->length() : face.length()); i < length; i++) this->points[i] ^= face.points[i];
			return *this;
		}
		Face operator+=(const point_t& pos) { for (point_t& point : points) point += pos; return *this; }
		Face operator+=(const num_t& var) { for (point_t& point : points) point += var; return *this; }
		Face operator+=(const Face<num_t, Size>& face) {
			for (int i = 0, length = int((this->length() < face.length()) ? this->length() : face.length()); i < length; i++) this->points[i] += face.points[i];
			return *this;
		}
		Face operator-=(const point_t& pos) { for (point_t& point : points) point -= pos; return *this; }
		Face operator-=(const num_t& var) { for (point_t& point : points) point -= var; return *this; }
		Face operator-=(const Face<num_t, Size>& face) {
			for (int i = 0, length = int((this->length() < face.length()) ? this->length() : face.length()); i < length; i++) this->points[i] -= face.points[i];
			return *this;
		}
		Face operator*=(const point_t& pos) { for (point_t& point : points) point *= pos; return *this; }
		Face operator*=(const num_t& var) { for (point_t& point : points) point *= var; return *this; }
		Face operator*=(const Face<num_t, Size>& face) {
			for (int i = 0, length = int((this->length() < face.length()) ? this->length() : face.length()); i < length; i++) this->points[i] *= face.points[i];
			return *this;
		}
		Face operator/=(const point_t& pos) { for (point_t& point : points) point /= pos; return *this; }
		Face operator/=(const num_t& var) { for (point_t& point : points) point /= var; return *this; }
		Face operator/=(const Face<num_t, Size>& face) {
			for (int i = 0, length = int((this->length() < face.length()) ? this->length() : face.length()); i < length; i++) this->points[i] /= face.points[i];
			return *this;
		}
		Face operator%=(const point_t& pos) { for (point_t& point : points) point %= pos; return *this; }
		Face operator%=(const num_t& var) { for (point_t& point : points) point %= var; return *this; }
		Face operator%=(const Face<num_t, Size>& face) {
			for (int i = 0, length = int((this->length() < face.length()) ? this->length() : face.length()); i < length; i++) this->points[i] %= face.points[i];
			return *this;
		}
		Face operator&=(const point_t& pos) { for (point_t& point : points) point &= pos; return *this; }
		Face operator&=(const num_t& var) { for (point_t& point : points) point &= var; return *this; }
		Face operator&=(const Face<num_t, Size>& face) {
			for (int i = 0, length = int((this->length() < face.length()) ? this->length() : face.length()); i < length; i++) this->points[i] &= face.points[i];
			return *this;
		}

#		define SET_INTO_NEW_SKFACE(opr, pos)											 \
		{ ret.points.add(this->points[i] opr pos);								\
		if constexpr(is_colored) ret.points[i].color = this->points[i].color;			\
		if constexpr(has_noramls) ret.points[i].noramls = this->points[i].noramls;		\
		if constexpr(is_textured) ret.points[i].texcoords = this->points[i].texcoords;	}

		Face operator^(const point_t& pos) {
			Face<num_t, Size, is_colored, has_noramls, is_textured, has_indices> ret;
			if constexpr(has_indices) ret.indices = this->indices;
			for (int i = 0, length = int(this->length()); i < length; i++) SET_INTO_NEW_SKFACE(^, pos)
			return ret;
		}
		Face operator^(const num_t& var) {
			Face<num_t, Size, is_colored, has_noramls, is_textured, has_indices> ret;
			if constexpr(has_indices) ret.indices = this->indices;
			for (int i = 0, length = int(this->length()); i < length; i++) SET_INTO_NEW_SKFACE(^, var)
			return ret;
		}
		Face operator^(const Face<num_t, Size>& face) {
			Face<num_t, Size, is_colored, has_noramls, is_textured, has_indices> ret;
			if constexpr(has_indices) ret.indices = this->indices;
			for (int i = 0, length = int((this->length() < face.length()) ? this->length() : face.length()); i < length; i++) SET_INTO_NEW_SKFACE(^, face.points[i])
			return ret;
		}
		Face operator+(const point_t& pos) {
			Face<num_t, Size, is_colored, has_noramls, is_textured, has_indices> ret;
			if constexpr(has_indices) ret.indices = this->indices;
			for (int i = 0, length = int(this->length()); i < length; i++) SET_INTO_NEW_SKFACE(+, pos)
			return ret;
		}
		Face operator+(const num_t& var) {
			Face<num_t, Size, is_colored, has_noramls, is_textured, has_indices> ret;
			if constexpr(has_indices) ret.indices = this->indices;
			for (int i = 0, length = int(this->length()); i < length; i++) SET_INTO_NEW_SKFACE(+, var)
			return ret;
		}
		Face operator+(const Face<num_t, Size>& face) {
			Face<num_t, Size, is_colored, has_noramls, is_textured, has_indices> ret;
			if constexpr(has_indices) ret.indices = this->indices;
			for (int i = 0, length = int((this->length() < face.length()) ? this->length() : face.length()); i < length; i++) SET_INTO_NEW_SKFACE(+, face.points[i])
			return ret;
		}
		Face operator-(const point_t& pos) {
			Face<num_t, Size, is_colored, has_noramls, is_textured, has_indices> ret;
			if constexpr(has_indices) ret.indices = this->indices;
			for (int i = 0, length = int(this->length()); i < length; i++) SET_INTO_NEW_SKFACE(-, pos)
			return ret;
		}
		Face operator-(const num_t& var) {
			Face<num_t, Size, is_colored, has_noramls, is_textured, has_indices> ret;
			if constexpr(has_indices) ret.indices = this->indices;
			for (int i = 0, length = int(this->length()); i < length; i++) SET_INTO_NEW_SKFACE(-, var)
			return ret;
		}
		Face operator-(const Face<num_t, Size>& face) {
			Face<num_t, Size, is_colored, has_noramls, is_textured, has_indices> ret;
			if constexpr(has_indices) ret.indices = this->indices;
			for (int i = 0, length = int((this->length() < face.length()) ? this->length() : face.length()); i < length; i++) SET_INTO_NEW_SKFACE(-, face.points[i])
			return ret;
		}
		Face operator*(const point_t& pos) {
			Face<num_t, Size, is_colored, has_noramls, is_textured, has_indices> ret;
			if constexpr(has_indices) ret.indices = this->indices;
			for (int i = 0, length = int(this->length()); i < length; i++) SET_INTO_NEW_SKFACE(*, pos)
			return ret;
		}
		Face operator*(const num_t& var) {
			Face<num_t, Size, is_colored, has_noramls, is_textured, has_indices> ret;
			if constexpr(has_indices) ret.indices = this->indices;
			for (int i = 0, length = int(this->length()); i < length; i++) SET_INTO_NEW_SKFACE(*, var)
			return ret;
		}
		Face operator*(const Face<num_t, Size>& face) {
			Face<num_t, Size, is_colored, has_noramls, is_textured, has_indices> ret;
			if constexpr(has_indices) ret.indices = this->indices;
			for (int i = 0, length = int((this->length() < face.length()) ? this->length() : face.length()); i < length; i++) SET_INTO_NEW_SKFACE(*, face.points[i])
			return ret;
		}
		Face operator/(const point_t& pos) {
			Face<num_t, Size, is_colored, has_noramls, is_textured, has_indices> ret;
			if constexpr(has_indices) ret.indices = this->indices;
			for (int i = 0, length = int(this->length()); i < length; i++) SET_INTO_NEW_SKFACE(/, pos)
			return ret;
		}
		Face operator/(const num_t& var) {
			Face<num_t, Size, is_colored, has_noramls, is_textured, has_indices> ret;
			if constexpr(has_indices) ret.indices = this->indices;
			for (int i = 0, length = int(this->length()); i < length; i++) SET_INTO_NEW_SKFACE(/, var)
			return ret;
		}
		Face operator/(const Face<num_t, Size>& face) {
			Face<num_t, Size, is_colored, has_noramls, is_textured, has_indices> ret;
			if constexpr(has_indices) ret.indices = this->indices;
			for (int i = 0, length = int((this->length() < face.length()) ? this->length() : face.length()); i < length; i++) SET_INTO_NEW_SKFACE(/, face.points[i])
			return ret;
		}
		Face operator%(const point_t& pos) {
			Face<num_t, Size, is_colored, has_noramls, is_textured, has_indices> ret;
			if constexpr(has_indices) ret.indices = this->indices;
			for (int i = 0, length = int(this->length()); i < length; i++) SET_INTO_NEW_SKFACE(%, pos)
			return ret;
		}
		Face operator%(const num_t& var) {
			Face<num_t, Size, is_colored, has_noramls, is_textured, has_indices> ret;
			if constexpr(has_indices) ret.indices = this->indices;
			for (int i = 0, length = int(this->length()); i < length; i++) SET_INTO_NEW_SKFACE(%, var)
			return ret;
		}
		Face operator%(const Face<num_t, Size>& face) {
			Face<num_t, Size, is_colored, has_noramls, is_textured, has_indices> ret;
			if constexpr(has_indices) ret.indices = this->indices;
			for (int i = 0, length = int((this->length() < face.length()) ? this->length() : face.length()); i < length; i++) SET_INTO_NEW_SKFACE(%, face.points[i])
			return ret;
		}
		Face operator&(const point_t& pos) {
			Face<num_t, Size, is_colored, has_noramls, is_textured, has_indices> ret;
			if constexpr(has_indices) ret.indices = this->indices;
			for (int i = 0, length = int(this->length()); i < length; i++) SET_INTO_NEW_SKFACE(&, pos)
			return ret;
		}
		Face operator&(const num_t& var) {
			Face<num_t, Size, is_colored, has_noramls, is_textured, has_indices> ret;
			if constexpr(has_indices) ret.indices = this->indices;
			for (int i = 0, length = int(this->length()); i < length; i++) SET_INTO_NEW_SKFACE(&, var)
			return ret;
		}
		Face operator&(const Face<num_t, Size>& face) {
			Face<num_t, Size, is_colored, has_noramls, is_textured, has_indices> ret;
			if constexpr(has_indices) ret.indices = this->indices;
			for (int i = 0, length = int((this->length() < face.length()) ? this->length() : face.length()); i < length; i++) SET_INTO_NEW_SKFACE(&, face.points[i])
			return ret;
		}

		using glmvec_t = glm::vec<Size, num_t, glm::defaultp>;
		Face operator^=(const glmvec_t& pos) requires(Size >= 2 and Size < 5) { for (point_t& point : points) point ^= pos; return *this; }
		Face operator+=(const glmvec_t& pos) requires(Size >= 2 and Size < 5) { for (point_t& point : points) point ^= pos; return *this; }
		Face operator-=(const glmvec_t& pos) requires(Size >= 2 and Size < 5) { for (point_t& point : points) point ^= pos; return *this; }
		Face operator*=(const glmvec_t& pos) requires(Size >= 2 and Size < 5) { for (point_t& point : points) point ^= pos; return *this; }
		Face operator/=(const glmvec_t& pos) requires(Size >= 2 and Size < 5) { for (point_t& point : points) point ^= pos; return *this; }
		Face operator%=(const glmvec_t& pos) requires(Size >= 2 and Size < 5) { for (point_t& point : points) point ^= pos; return *this; }
		Face operator&=(const glmvec_t& pos) requires(Size >= 2 and Size < 5) { for (point_t& point : points) point ^= pos; return *this; }

		Face operator^(const glmvec_t& pos) requires(Size == 2 and Size < 5) {
			Face<num_t, Size, is_colored, has_noramls, is_textured, has_indices> ret;
			if constexpr(has_indices) ret.indices = this->indices;
			for (int i = 0, length = int(this->length()); i < length; i++) SET_INTO_NEW_SKFACE(^, pos)
			return ret;
		}
		Face operator+(const glmvec_t& pos) requires(Size == 2 and Size < 5) {
			Face<num_t, Size, is_colored, has_noramls, is_textured, has_indices> ret;
			if constexpr(has_indices) ret.indices = this->indices;
			for (int i = 0, length = int(this->length()); i < length; i++) SET_INTO_NEW_SKFACE(+, pos)
			return ret;
		}
		Face operator-(const glmvec_t& pos) requires(Size == 2 and Size < 5) {
			Face<num_t, Size, is_colored, has_noramls, is_textured, has_indices> ret;
			if constexpr(has_indices) ret.indices = this->indices;
			for (int i = 0, length = int(this->length()); i < length; i++) SET_INTO_NEW_SKFACE(-, pos)
			return ret;
		}
		Face operator*(const glmvec_t& pos) requires(Size == 2 and Size < 5) {
			Face<num_t, Size, is_colored, has_noramls, is_textured, has_indices> ret;
			if constexpr(has_indices) ret.indices = this->indices;
			for (int i = 0, length = int(this->length()); i < length; i++) SET_INTO_NEW_SKFACE(*, pos)
			return ret;
		}
		Face operator/(const glmvec_t& pos) requires(Size == 2 and Size < 5) {
			Face<num_t, Size, is_colored, has_noramls, is_textured, has_indices> ret;
			if constexpr(has_indices) ret.indices = this->indices;
			for (int i = 0, length = int(this->length()); i < length; i++) SET_INTO_NEW_SKFACE(/, pos)
			return ret;
		}
		Face operator%(const glmvec_t& pos) requires(Size == 2 and Size < 5) {
			Face<num_t, Size, is_colored, has_noramls, is_textured, has_indices> ret;
			if constexpr(has_indices) ret.indices = this->indices;
			for (int i = 0, length = int(this->length()); i < length; i++) SET_INTO_NEW_SKFACE(%, pos)
			return ret;
		}
		Face operator&(const glmvec_t& pos) requires(Size == 2 and Size < 5) {
			Face<num_t, Size, is_colored, has_noramls, is_textured, has_indices> ret;
			if constexpr(has_indices) ret.indices = this->indices;
			for (int i = 0, length = int(this->length()); i < length; i++) SET_INTO_NEW_SKFACE(&, pos)
			return ret;
		}
#		undef SET_INTO_NEW_SKFACE

		void operator <<(const point_t& pos) { this->points.add(pos); }

		Arrays::Array<line_t> get_lines() {
			Arrays::Array<line_t> ret = Arrays::Array<line_t>();
			for (int i = 0, size = int(this->points.size()); i < size; i++)
				if (i == size - 1) ret.add(line_t(points[i], points[0]));
				else ret.add(line_t(points[i], points[(i + 1) % size]));
			return ret;
		}
		
		point_t get_center() {
			point_t pos = {};
			std::size_t size = this->points.size();
			for (std::size_t i = 0; i < size; i++) pos += this->points[i];
			return point_t(pos.x / size, pos.y / size);
		}

		Rect<num_t, Size> get_rect() requires (Size > 1 and Size < 5) {
			Rect<num_t, Size> ret;
			for (point_t& point : this->points) {
				if (ret.x < point.x) ret.x = point.x;
				if (ret.y > point.y) ret.y = point.y;
				if constexpr(Size >= 3 && Size <= 4) if (ret.z > point.z) ret.z = point.z;
				if constexpr(Size == 4) if (ret.w > point.w) ret.w = point.w;
				if (ret.width > point.x) ret.width = point.x;
				if (ret.height < point.y) ret.height = point.y;
				if constexpr(Size >= 3 && Size <= 4) if (ret.length < point.z) ret.length = point.z;
				if constexpr(Size == 4) if (ret.power < point.w) ret.power = point.w;
			} return ret;
		}

		Rect<num_t, Size> get_rect() requires (Size > 4) {
			Rect<num_t, Size> ret;
			for (point_t& point : this->points) {
				if (ret.data[0] < point.data[0]) ret.data[0] = this->point.data[0];
				for (int i = 1; i < Size; i++) if (ret.data[i] > point.data[i]) ret.data[i] = this->point.data[i];
				if (ret.data[Size] > point.data[0]) ret.data[Size] = this->point.data[0];
				for (int i = Size+1; i < Size*2; i++) if (ret.data[i] < point.data[i/2]) ret.data[i] = this->point.data[i/2];
			} return ret;
		}

		Point<num_t, Size> get_leftop_corner() requires (Size > 1 and Size < 5) {
			Point<num_t, Size> ret;
			for (point_t& point : this->points) {
				if (ret.x < point.x) ret.x = point.x;
				if (ret.y > point.y) ret.y = point.y;
				if constexpr(Size >= 3 && Size <= 4) if (ret.z > point.z) ret.z = point.z;
				if constexpr(Size == 4) if (ret.w > point.w) ret.w = point.w;
			} return ret;
		}
		Point<num_t, Size> get_leftop_corner() requires (Size > 4) {
			Point<num_t, Size> ret;
			for (point_t& point : this->points) {
				if (ret.data[0] < point.data[0]) ret.data[0] = this->point.data[0];
				for (int i = 1; i < Size; i++) if (ret.data[i] > point.data[i]) ret.data[i] = this->point.data[i];
			} return ret;
		}
		void set_leftop_corner(const point_t& pos) {
			Point<num_t, Size> d = pos - this->get_leftop_corner();
			for (point_t& point : this->points) point += d;
		}

		Point<num_t, Size> get_rightbuttom_corner() requires (Size > 1 and Size < 5) {
			Point<num_t, Size> ret;
			for (point_t& point : this->points) {
				if (ret.x > point.x) ret.x = point.x;
				if (ret.y < point.y) ret.y = point.y;
				if constexpr(Size >= 3 && Size <= 4) if (ret.z < point.z) ret.z = point.z;
				if constexpr(Size == 4) if (ret.w < point.w) ret.w = point.w;
			} return ret;
		}
		Point<num_t, Size> get_rightbuttom_corner() requires (Size > 4) {
			Point<num_t, Size> ret;
			for (point_t& point : this->points) {
				if (ret.data[0] > point.data[0]) ret.data[0] = this->point.data[0];
				for (int i = 1; i < Size; i++) if (ret.data[i] < point.data[i]) ret.data[i] = this->point.data[i];
			} return ret;
		}
		void set_rightbuttom_corner(const point_t& pos) {
			Point<num_t, Size> d = pos - this->get_rightbuttom_corner();
			for (point_t& point : this->points) point += d;
		}

		Point<num_t, Size> get_distances() {
			Point<num_t, Size> ret;
			for (int i = 0, size = int(this->points.size()); i < size; i++)
				if (i == size - 1) ret.add(points[0] - points[i]);
				else ret.add(points[(i + 1) % size] - points[i]);
			return ret;
		} 

		Face mirror(bool mirror = true, bool set_this_to = false) const {
			Face<num_t, Size, is_colored, has_noramls, is_textured, has_indices> ret;
			for (int i = 0; i < this->points.size(); i++) 
				if (set_this_to) ret.points.add(this->points[i] = this->points[i].mirror(mirror, set_this_to));
				else ret.points.add(this->points[i].mirror(mirror, set_this_to));
			return ret;
		}

		bool is_touched(const point_t& pA, const point_t& pB) requires (Size == 2 or Size == 3) { return this->is_touched({ pA, pB }); }
		bool is_touched(const line_t& line_) requires (Size == 2 or Size == 3) {
			bool ret = false;
			Arrays::Array<line_t> lines = get_lines();
			for (line_t line : lines) if (line.is_touched(line_)) ret = true;
			return ret;
		}

		std::string tostring(std::string spliter = ",") requires (has_indices) { return tostring(spliter, "\n", ";\n"); }
		std::string tostring(std::string spliter = ",") requires (!has_indices) { return tostring(spliter, "\n"); }
		std::string tostring(std::string vec_spliter , std::string point_spliter, std::string indices_spliter) requires (has_indices) {
			std::string ret;
			for (int i = 0, size = int(this->points.size()); i < size; i++) ret += this->points[i].tostring(vec_spliter) + ((i == size - 1) ? "" : point_spliter);
			ret += indices_spliter;
			for (int i = 0, size = int(this->indices.size()); i < size; i++) ret += std::to_string(this->indices[i]) + ((i == size - 1) ? "" : point_spliter);
			return ret;
		}
		std::string tostring(std::string vec_spliter, std::string point_spliter) requires (!has_indices) {
			std::string ret;
			for (int i = 0, size = int(this->points.size()); i < size; i++) ret += this->points[i].tostring(vec_spliter) + ((i == size - 1) ? "" : point_spliter);
			return ret;
		}
		void fromfile(std::string path, std::string spliter = ",") { fromstring(file::get_contents(path.c_str()), spliter); }
		void tofile(std::string path, std::string spliter = ",") { file::save(path.c_str(), this->tostring().c_str()); }
		void print() { std::cout << this->tostring() << std::endl; }
		void fromstring(std::string str, std::string spliter = ",") requires (!has_indices) {
			std::vector<std::string> strs = str::split(str, "\n");
			for (std::string str : strs) {
				point_t pos;
				pos.fromstring(str, spliter);
				this->points.add(pos);
			}
		}
		void fromstring(std::string str, std::string spliter = ",") requires (has_indices) {
			std::vector<std::string> blocks = str::split(str, ";\n"), strs = str::split(blocks[0], "\n"), indices_strs = str::split(blocks[1], "\n");
			for (std::string str : indices_strs) this->indices.add(TXT_TO_NUM<num_t>(str));
			for (std::string str : strs) {
				point_t pos;
				pos.fromstring(str, spliter);
				this->points.add(pos);
			}
		}

		std::wstring towstring(std::wstring spliter = L",") requires (has_indices) { return towstring(spliter, L"\n", L";\n"); }
		std::wstring towstring(std::wstring spliter = L",") requires (!has_indices) { return towstring(spliter, L"\n"); }
		std::wstring towstring(std::wstring vec_spliter, std::wstring point_spliter, std::wstring indices_spliter) requires (has_indices) {
			std::wstring ret;
			for (int i = 0, size = int(this->points.size()); i < size; i++) ret += this->points[i].towstring(vec_spliter) + ((i == size - 1) ? L"" : point_spliter);
			ret += indices_spliter;
			for (int i = 0, size = int(this->indices.size()); i < size; i++) ret += std::to_wstring(this->indices[i]) + ((i == size - 1) ? L"" : point_spliter);
			return ret;
		}
		std::wstring towstring(std::wstring vec_spliter, std::wstring point_spliter) requires (!has_indices) {
			std::wstring ret;
			for (int i = 0, size = int(this->points.size()); i < size; i++) ret += this->points[i].towstring(vec_spliter) + ((i == size - 1) ? L"" : point_spliter);
			return ret;
		}

		void wprint() { std::wcout << this->towstring() << std::endl; }
		void fromwstring(std::wstring str, std::wstring spliter = L",") requires (!has_indices) {
			std::vector<std::wstring> strs = str::split(str, L"\n");
			for (std::wstring str : strs) {
				point_t pos;
				pos.fromwstring(str, spliter);
				this->points.add(pos);
			}
		}
		void fromwstring(std::wstring str, std::wstring spliter = L",") requires (has_indices) {
			std::vector<std::wstring> blocks = str::split(str, L";\n"), strs = str::split(blocks[0], L"\n"), indices_strs = str::split(blocks[1], L"\n");
			for (std::wstring str : indices_strs) this->indices.add(TXT_TO_NUM<num_t>(str));
			for (std::wstring str : strs) {
				point_t pos;
				pos.fromwstring(str, spliter);
				this->points.add(pos);
			}
		}

	// will be add next

		static Face Triangle(point_t p1, point_t p2, point_t p3) { return Face<num_t, Size, is_colored, has_noramls, is_textured, has_indices>({ p1, p2, p3 }); }

		template <typename... Args> static Face Triangle(Args... args) requires (sizeof...(Args) == 6 and Size == 2) {
			auto [x1, y1, x2, y2, x3, y3] = std::make_tuple(args...);
			return Triangle(point_t(x1, y1), point_t(x2, y2), point_t(x3, y3));
		}
		template <typename... Args> static Face Triangle(Args... args) requires (sizeof...(Args) == 9 and Size == 3) {
			auto [x1, y1, z1, x2, y2, z2, x3, y3, z3] = std::make_tuple(args...);
			return Triangle(point_t(x1, y1, z1), point_t(x2, y2, z2), point_t(x3, y3, z3));
		}

		static Face Circle(num_t cx, num_t cy, num_t cz, num_t rx, num_t ry, num_t rz, int segments = 36) requires (Size == 3) { return Polygon(cx, cy, cz, rx, ry, rz, segments); }
		static Face Circle(point_t center, point_t radius, int segments = 36) requires (Size == 2 or Size == 3) { return Polygon(center, radius, segments); }
		static Face Circle(num_t cx, num_t cy, num_t rx, num_t ry, int segments = 36) requires (Size == 2) { return Polygon(cx, cy, rx, ry, segments); }

		static Face Polygon(num_t cx, num_t cy, num_t cz, num_t rx, num_t ry, num_t rz, int sides) requires (Size == 3) { return Polygon({ cx, cy, cz }, { rx, ry, rz }, sides); }
		static Face Polygon(num_t cx, num_t cy, num_t rx, num_t ry, int sides) requires (Size == 2) { return Polygon({ cx, cy }, { rx, ry }, sides); }
		static Face Polygon(point_t center, point_t radius, int sides) requires (Size == 2 or Size == 3) {
			Face<num_t, Size, is_colored, has_noramls, is_textured, has_indices> ret;
			const num_t angle_step = 2 * M_PI / sides;
			for (int i = 0; i < sides; ++i) {
				const num_t angle = i * angle_step;
				ret.points.add(center + radius * ((Size == 2) ? point_t{ std::cos(angle), std::sin(angle) } : point_t{ std::cos(angle), std::sin(angle) , 1}));
			} return ret;
		}

		static Face Square(point_t position, num_t size) requires (Size == 2 or Size == 3) { return Rectangle(position, (Size == 2) ? point_t(size, size) : point_t(size, size, size)); }

		static Face Rectangle_Front(point_t position, point_t size) requires (Size == 3) { return Rectangle(position, size); }

		static Face Rectangle_Back(point_t position, point_t size, num_t depth) requires (Size == 3) {
			auto face = Rectangle(position, size);
			for (point_t& p : face.points) p.z += depth;
			return face;
		}

		static Face Rectangle(point_t position, point_t size) requires (Size == 2 or Size == 3) {
			Face<num_t, Size, is_colored, has_noramls, is_textured, has_indices> ret;
			if constexpr (Size == 2) ret.points = {
				{ position.x,		  position.y							  },
				{ position.x,		  position.y + size.y					 },
				{ position.x + size.x, position.y + size.y					 },
				{ position.x + size.x, position.y							  }
			}; else ret.points = {
				{ position.x,		  position.y,		  position.z		 },
				{ position.x,		  position.y + size.y, position.z		 },
				{ position.x + size.x, position.y + size.y, position.z		 },
				{ position.x + size.x, position.y,		  position.z		 }
			}; return ret;
		}

		static Face Rectangle_Left(point_t position, point_t size, num_t depth) requires (Size == 3) {
			return Face<num_t, Size, is_colored, has_noramls, is_textured, has_indices>({
				{ position.x		 , position.y		 , position.z		 },
				{ position.x		 , position.y + size.y, position.z		 },
				{ position.x		 , position.y + size.y, position.z + depth },
				{ position.x		 , position.y		 , position.z + depth }
			});
		}

		static Face Rectangle_Right(point_t position, point_t size, num_t depth) requires (Size == 3) {
			return Face<num_t, Size, is_colored, has_noramls, is_textured, has_indices>({
				{ position.x + size.x, position.y		 , position.z		 },
				{ position.x + size.x, position.y + size.y, position.z		 },
				{ position.x + size.x, position.y + size.y, position.z + depth },
				{ position.x + size.x, position.y		 , position.z + depth }
			});
		}

		static Face Rectangle_Top(point_t position, point_t size, num_t depth) requires (Size == 3) {
			return Face<num_t, Size, is_colored, has_noramls, is_textured, has_indices>({
				{ position.x		 , position.y + size.y, position.z		 },
				{ position.x		 , position.y + size.y, position.z + depth },
				{ position.x + size.x, position.y + size.y, position.z + depth },
				{ position.x + size.x, position.y + size.y, position.z		 }
			});
		}

		static Face Rectangle_Buttom(point_t position, point_t size, num_t depth) requires (Size == 3) {
			return Face<num_t, Size, is_colored, has_noramls, is_textured, has_indices>({
				{ position.x		 , position.y		 , position.z		 },
				{ position.x		 , position.y		 , position.z + depth },
				{ position.x + size.x, position.y		 , position.z + depth },
				{ position.x + size.x, position.y		 , position.z		 }
			});
		}

		static inline void set_point_callback(void* data) requires(Size == 2 or Size == 3) {
			point_t* p = static_cast<point_t*>(data);
			p->set_glpos();
			if constexpr (is_colored) p->color.set_glcolor();
			if constexpr (has_noramls) p->color.set_glnoraml();
			//if constexpr (is_textured) ;
		}
		void Show(num_t mx = 0, num_t my = 0, Point<num_t, 4> color = {-1,1,1,1}) requires(!is_colored and Size == 2) {if (this->points.size() > 0) {
			GLUtesselator* tess = gluNewTess();
			gluTessCallback(tess, GLU_TESS_BEGIN, (void(*)())glBegin);
			gluTessCallback(tess, GLU_TESS_VERTEX, (void(*)())set_point_callback);
			gluTessCallback(tess, GLU_TESS_END, (void(*)())glEnd);
			gluTessBeginPolygon(tess, nullptr);
			  gluTessBeginContour(tess);
					  color.set_glcolor();
						if constexpr (std::is_same_v<num_t, double>)
						  for (point_t& p : this->points)  {
							auto dp = (p + point_t{mx, my});
							double data[3] = {dp.x, dp.y, 0};
							gluTessVertex(tess, data, &dp);
						} else for (point_t& p : this->points) {
							auto dp = (p + point_t{mx, my}).template cast<double>();
							double data[3] = {dp.x, dp.y, 0};
							gluTessVertex(tess, data, &dp);
						}
					  gluTessEndContour(tess);
					gluTessEndPolygon(tess);
					gluDeleteTess(tess);
		}}

		void Show(num_t mx = 0, num_t my = 0) requires(is_colored and Size == 2) {if (!this->points.empty()) {
			GLUtesselator* tess = gluNewTess();
			gluTessCallback(tess, GLU_TESS_BEGIN, (void(*)())glBegin);
			gluTessCallback(tess, GLU_TESS_VERTEX, (void(*)())set_point_callback);
			gluTessCallback(tess, GLU_TESS_END, (void(*)())glEnd);
			gluTessBeginPolygon(tess, nullptr);
			  gluTessBeginContour(tess);
					if constexpr (std::is_same_v<num_t, double>)
					  for (point_t& p : this->points)  {
						auto dp = (p + point_t{mx, my});
						p.color.set_glcolor();
						double data[3] = {dp.x, dp.y, 0};
						gluTessVertex(tess, data, &data[0]);
					} else for (point_t& p : this->points) {
						auto dp = (p + point_t{mx, my}).template cast<double>();
						p.color.set_glcolor();
						double data[3] = {dp.x, dp.y, 0};
						gluTessVertex(tess, data, &data[0]);
					}
				  gluTessEndContour(tess);
				gluTessEndPolygon(tess);
				gluDeleteTess(tess);
		}}
		template<int setting = 0x0006, int algs_num = 360, int step = 1>
		  inline void show_points(num_t rx = num_t(0.01f), num_t ry = num_t(0.01f), Point<num_t, 4> color = {-1,1,1,1}) requires(Size == 2 and !is_colored)
			{ for (point_t& p : this->points) p.template show<setting, algs_num, step>(rx, ry, color); }
		template<int setting = 0x0006, int algs_num = 360, int step = 1>

		  inline void show_points(num_t rx = num_t(0.01f), num_t ry = num_t(0.01f)) requires(Size == 2 and is_colored)
			{ for (point_t& p : this->points) p.template show<setting, algs_num, step>(rx, ry); }
		template<int setting = 0x0006, int algs_num = 360, int step = 1>
		  inline void show_points(num_t rx, num_t ry, std::vector<Point<num_t, 4>> colors) requires(Size == 2)
			{ for (point_t& p : this->points) p.template show<setting, algs_num, step>(rx, ry, colors); }

		template<int setting = 0x0006, float line_size = -1.0f>
		inline void show(num_t mx = 0, num_t my = 0, Point<num_t, 4> color = {-1,1,1,1}) requires(Size == 2 and !is_colored) {
			if constexpr (line_size > 0) glLineWidth(line_size);
			color.set_glcolor();
			glBegin(setting); for (point_t& p : this->points) {
			  p.set_glpos(mx, my);
			} glEnd();
		}
		template<int setting = 0x0006, float line_size = -1.0f>
		inline void show(num_t mx = 0, num_t my = 0) requires(Size == 2 and is_colored) {
			if constexpr (line_size > 0) glLineWidth(line_size);
			glBegin(setting); for (point_t& p : this->points) {
			  p.color.set_glcolor();
			  p.set_glpos(mx, my);
			} glEnd();
		}
		template<int setting = 0x0006, float line_size = -1.0f>
		inline void show(num_t mx, num_t my, std::vector<Point<num_t, 4>> colors) requires(Size == 2) {
			if constexpr (line_size > 0) glLineWidth(line_size);
			glBegin(setting); for (std::size_t i = 0; i < this->points.size(); i++) {
			  if (i < colors.size()) colors[i].set_glcolor();
			  this->points[i].set_glpos(mx, my);
			} glEnd();
		}
	};

	template<typename num_t> using Face2D	  = Face<num_t, 2, false, false, false, false>;
	template<typename num_t> using Face3D	  = Face<num_t, 3, false, false, false, false>;
	template<typename num_t> using Face4D	  = Face<num_t, 4, false, false, false, false>;
	template<typename num_t> using TFace2D	 = Face<num_t, 2, false, false, true , false>;
	template<typename num_t> using TFace3D	 = Face<num_t, 3, false, false, true , false>;
	template<typename num_t> using TFace4D	 = Face<num_t, 4, false, false, true , false>;
	template<typename num_t> using CFace2D	 = Face<num_t, 2, true , false, false, false>;
	template<typename num_t> using CFace3D	 = Face<num_t, 3, true , false, false, false>;
	template<typename num_t> using CFace4D	 = Face<num_t, 4, true , false, false, false>;
	template<typename num_t> using CTFace2D	= Face<num_t, 2, true , false, true , false>;
	template<typename num_t> using CTFace3D	= Face<num_t, 3, true , false, true , false>;
	template<typename num_t> using CTFace4D	= Face<num_t, 4, true , false, true , false>;
	template<typename num_t> using Polygon2D   = Face<num_t, 2, false, true , false, false>;
	template<typename num_t> using Polygon3D   = Face<num_t, 3, false, true , false, false>;
	template<typename num_t> using Polygon4D   = Face<num_t, 4, false, true , false, false>;
	template<typename num_t> using CPolygon2D  = Face<num_t, 2, true , true , false, false>;
	template<typename num_t> using CPolygon3D  = Face<num_t, 3, true , true , false, false>;
	template<typename num_t> using CPolygon4D  = Face<num_t, 4, true , true , false, false>;
	template<typename num_t> using Shape2D	 = Face<num_t, 2, false, false, false, true >;
	template<typename num_t> using Shape3D	 = Face<num_t, 3, false, false, false, true >;
	template<typename num_t> using Shape4D	 = Face<num_t, 4, false, false, false, true >;
	template<typename num_t> using CShape2D	= Face<num_t, 2, true , false, false, true >;
	template<typename num_t> using CShape3D	= Face<num_t, 3, true , false, false, true >;
	template<typename num_t> using CShape4D	= Face<num_t, 4, true , false, false, true >;
	template<typename num_t> using CTShape2D   = Face<num_t, 2, true , false, true , true >;
	template<typename num_t> using CTShape3D   = Face<num_t, 3, true , false, true , true >;
	template<typename num_t> using CTShape4D   = Face<num_t, 4, true , false, true , true >;
	template<typename num_t> using PolyShape2D = Face<num_t, 2, false, true , false, true >;
	template<typename num_t> using PolyShape3D = Face<num_t, 3, false, true , false, true >;
	template<typename num_t> using PolyShape4D = Face<num_t, 4, false, true , false, true >;
	template<typename num_t> using Model2D	 = Face<num_t, 2, true , true , false, true >;
	template<typename num_t> using Model3D	 = Face<num_t, 3, true , true , false, true >;
	template<typename num_t> using Model4D	 = Face<num_t, 4, true , true , false, true >;
	template<typename num_t> using Mesh2D	  = Face<num_t, 2, true , true , true , true >;
	template<typename num_t> using Mesh3D	  = Face<num_t, 3, true , true , true , true >;
	template<typename num_t> using Mesh4D	  = Face<num_t, 4, true , true , true , true >;
	using FaceF2D	  = Face<float, 2, false, false, false, false>;
	using FaceF3D	  = Face<float, 3, false, false, false, false>;
	using FaceF4D	  = Face<float, 4, false, false, false, false>;
	using TFaceF2D	 = Face<float, 2, false, false, true , false>;
	using TFaceF3D	 = Face<float, 3, false, false, true , false>;
	using TFaceF4D	 = Face<float, 4, false, false, true , false>;
	using CFaceF2D	 = Face<float, 2, true , false, false, false>;
	using CFaceF3D	 = Face<float, 3, true , false, false, false>;
	using CFaceF4D	 = Face<float, 4, true , false, false, false>;
	using CTFaceF2D	= Face<float, 2, true , false, true , false>;
	using CTFaceF3D	= Face<float, 3, true , false, true , false>;
	using CTFaceF4D	= Face<float, 4, true , false, true , false>;
	using PolygonF2D   = Face<float, 2, false, true , false, false>;
	using PolygonF3D   = Face<float, 3, false, true , false, false>;
	using PolygonF4D   = Face<float, 4, false, true , false, false>;
	using CPolygonF2D  = Face<float, 2, true , true , false, false>;
	using CPolygonF3D  = Face<float, 3, true , true , false, false>;
	using CPolygonF4D  = Face<float, 4, true , true , false, false>;
	using ShapeF2D	 = Face<float, 2, false, false, false, true >;
	using ShapeF3D	 = Face<float, 3, false, false, false, true >;
	using ShapeF4D	 = Face<float, 4, false, false, false, true >;
	using CShapeF2D	= Face<float, 2, true , false, false, true >;
	using CShapeF3D	= Face<float, 3, true , false, false, true >;
	using CShapeF4D	= Face<float, 4, true , false, false, true >;
	using CTShapeF2D   = Face<float, 2, true , false, true , true >;
	using CTShapeF3D   = Face<float, 3, true , false, true , true >;
	using CTShapeF4D   = Face<float, 4, true , false, true , true >;
	using PolyShapeF2D = Face<float, 2, false, true , false, true >;
	using PolyShapeF3D = Face<float, 3, false, true , false, true >;
	using PolyShapeF4D = Face<float, 4, false, true , false, true >;
	using ModelF2D	 = Face<float, 2, true , true , false, true >;
	using ModelF3D	 = Face<float, 3, true , true , false, true >;
	using ModelF4D	 = Face<float, 4, true , true , false, true >;
	using MeshF2D	  = Face<float, 2, true , true , true , true >;
	using MeshF3D	  = Face<float, 3, true , true , true , true >;
	using MeshF4D	  = Face<float, 4, true , true , true , true >;
}
