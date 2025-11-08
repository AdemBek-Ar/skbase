module;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#undef __DEPRECATED

export module skbase.graph.camera;

import skbase.base;
import skbase.json;
import skbase.files;
import skbase.str.String;
import skbase.geomath;
import skbase.graph.shader;

import std;

export namespace graph {
	template <typename num_t=float, std::size_t Size=3>
	requires ((Size == 2 or Size == 3) and std::is_arithmetic_v<num_t>)
	class Camera : public geometry::Point<num_t, Size, false, false, false> {
	using point_t = geometry::Point<num_t, Size, false, false, false>;
	public :
		point_t aim;
		float FOV;
		Camera(point_t pos = 0, point_t aim = 0, float FOV = 45)
			: point_t(pos.x, pos.y, pos.z), aim(aim), FOV(FOV) { }

		glm::mat4 get_projection(float width, float height, float zNear = 0.1f, float zFar = 100)
			{ return glm::perspective(glm::radians(FOV), width / height, zNear, zFar); }
		glm::mat4 get_view(geometry::PointF3D a= {0,1,0})
			{ return glm::lookAt(this->glm_cast(), this->aim.glm_cast(), a.glm_cast()); }

		inline void set_position(const point_t& pos) { for (int i = 0; i < Size; i++) this->data[i] = pos.data[i]; }
		inline void set_position(float pos[3]) { for (int i = 0; i < 3; i++) this->data[i] = pos[i]; }
		inline void set_position(float x, float y, float z) {
			this->x = x;
			this->y = y;
			this->z = z;
		}

		template <bool move_aim = false, bool move_self = true>
		inline void move(point_t pos) {
			if constexpr (move_self)
				this->operator+=(pos);
			if constexpr (move_aim)
				aim += pos;
		}

		std::string tostring(std::string spliter=",")
			{ return point_t::tostring() + spliter + this->aim.tostring() + spliter + std::to_string(FOV); }
		void fromstring(std::string str, std::string spliter=",")  {
			std::vector<str_t> strs = str_t(str).split(spliter);
			for (ushort i=0; i<Size and i<strs.size(); i++)
				this->data[i] = strs[i].tofloat();
			for (ushort i=Size; i<Size*2 and i<strs.size(); i++)
				this->aim.data[i] = strs[i].tofloat();
			this->FOV = strs[2*Size].tofloat();
		}
		std::vector<byte> tobytes(std::string spliter=",")  {
			std::vector<byte> ret;
			std::string th_str = tostring();
			for(char ch : th_str)
				ret.push_back(static_cast<byte>(ch));
			return ret;
		}
		void frombytes(std::vector<byte> bytes, std::string spliter=",")  {
			std::string reading_from="";
			for(byte b : bytes)
				reading_from += static_cast<char>(b);
			fromstring(reading_from);
		}
		void tofile(std::string path, std::string spliter=",")  { file::save(path.c_str(), tostring().c_str()); }
		void fromfile(std::string path, std::string spliter=",")  { fromstring(file::get_contents(path.c_str()), spliter); }
		void print()  { std::cout << tostring() << std::endl; }

		std::ofstream tojsonfile(std::string path) { return file::save(path.c_str(), tojsonstr().c_str()); }
		void fromjsonfile (std::string path) { fromjsonstr(file::get_contents(path.c_str())); }

		std::string tojsonstr() { return tojson().dump(4); }
		void fromjsonstr(std::string str)  {
		  json j = json::parse(str);
		  fromjson(j);
		}
		json tojson()  {
			json j;
			j["x"]   = this->x;
			j["y"]   = this->y;
			j["z"]   = this->z;
			j["aim"] = this->aim.tojson();
			j["FOV"] = this->FOV;
			return j;
		}
		void fromjson(json j)  {
			this->x = j["x"];
			this->y = j["y"];
			this->z = j["z"];
			this->aim.fromjson(j["aim"]);
			this->FOV = j["FOV"];
		}
		template <
				uint SCR_WIDTH, uint SCR_HEIGHT,
			   const char* posn_key = "viewPos",
			   const char* view_key = "view",
			   const char* prjc_key = "projection">
		inline void active(Shader shader = Shader::skbase_default_shader()) {
            shader( prjc_key , this->get_projection(SCR_WIDTH, SCR_HEIGHT));
            shader( view_key , this->get_view()                           );
            shader( posn_key , this->glm_cast()                           );
		}
	};



	template <typename num_t>
	using Camera2D  = Camera<num_t, 2>;
	template <typename num_t>
	using Camera3D  = Camera<num_t, 3>;

	using CameraF2D = Camera<float, 2>;
	using CameraF3D = Camera<float, 3>;
}
