module;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#undef __DEPRECATED

export module skbase.graph.light;

import skbase.base;
import skbase.files;
import skbase.geomath;
import skbase.graph.shader;

import std;

export namespace graph {
	template <typename num_t=float, std::size_t Size=3>
	requires ((Size == 2 or Size == 3) and std::is_arithmetic_v<num_t>)
	class Lamp : public geometry::Point<num_t, Size, false, false> {
	using point_t = geometry::Point<num_t, Size, false, false, false>;
	public :
		union {
			geometry::Point4D<num_t> color;
			struct {
				num_t rgb[3];
				num_t intensity;
			};
		}; float ambientStrength, specularStrength;

		inline Lamp(
				point_t pos = {0,0,3},
				geometry::Point3D<num_t> rgb = {1,1,1},
				num_t intensity = 1,
				float ambientStrength = 0.2f,
				float specularStrength = 0.5f) :
			geometry::PointF3D(pos),
			color(rgb[0], rgb[1], rgb[2], intensity),
			ambientStrength(ambientStrength),
			specularStrength(specularStrength) {}

		inline Lamp(
				point_t pos,
				geometry::PointF4D color,
				float ambientStrength = 0.2f,
				float specularStrength = 0.5f) :
			point_t(pos), color(color),
			ambientStrength(ambientStrength),
			specularStrength(specularStrength) {}

		template <
			   const char* pos_key = "lightPos",
			   const char* clr_key = "lightColor",
			   const char* amb_key = "ambientStrength",
			   const char* spl_key = "specularStrength">
		inline void active(Shader shader = Shader::skbase_default_shader()) {
			shader(pos_key   , this->      glm_cast());
			shader(clr_key   , this->color.glm_cast());
			shader(amb_key   , this->ambientStrength );
			shader(spl_key   , this->specularStrength);
		}
	};
}
