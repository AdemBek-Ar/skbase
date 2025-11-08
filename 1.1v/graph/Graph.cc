module;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#undef __DEPRECATED

export module skbase.graph.main;

import skbase.base;
import skbase.geomath;
import skbase.memory.array;
import skbase.input;

import skbase.graph.shader;
import skbase.graph.camera;
import skbase.graph.light;

import std;



export namespace graph {
	template<typename num_t> requires (std::is_arithmetic_v<num_t>) using RGBColor = geometry::Point<num_t, 3, false, false, false>;
	template<typename num_t> requires (std::is_arithmetic_v<num_t>) using Color	   = geometry::Point<num_t, 4, false, false, false>;
	using RGBColorF = RGBColor<float>;
	using ColorF	   = Color   <float>;



	constexpr ColorF White     = ColorF{1   ,1   ,1   ,1};
	constexpr ColorF Red       = ColorF{1   ,0   ,0   ,1};
	constexpr ColorF Green     = ColorF{0   ,1   ,0   ,1};
	constexpr ColorF Blue      = ColorF{0   ,0   ,1   ,1};
	constexpr ColorF Yallow    = ColorF{1   ,1   ,0   ,1};
	constexpr ColorF Pinke     = ColorF{1   ,0   ,1   ,1};
	constexpr ColorF Orange    = ColorF{1   ,0.5f,0   ,1};
	constexpr ColorF Gray      = ColorF{0.5f,0.5f,0.5f,1};
	constexpr ColorF LightGray = ColorF{0.8f,0.8f,0.8f,1};
	constexpr ColorF Cyan      = ColorF{0   ,1   ,1   ,1};
	constexpr ColorF LightBlue = ColorF{0   ,0.5f,1   ,1};
	constexpr ColorF Black     = ColorF{0   ,0   ,0   ,1};



	template<bool has_indices = false> class ShapeLinkerIndices_extantion;
	template<> class ShapeLinkerIndices_extantion<false> {};
	template<> class ShapeLinkerIndices_extantion<true> {
	public : unsigned int EBO;
		ShapeLinkerIndices_extantion (std::vector<unsigned int> indices) { build(indices); }
		~ShapeLinkerIndices_extantion() { glDeleteVertexArrays(1, &EBO); }
		ShapeLinkerIndices_extantion () {}
		inline void build(std::vector<uint> indices) {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * indices.size(), indices.data(), GL_STATIC_DRAW);
		}
	};

	template<
		typename num_t = float,
		size_t Size = 3,
		bool is_colored = false,
		bool has_noramls = false,
		bool is_textured = false,
		bool has_indices = false>
	class ShapeLinker : public ShapeLinkerIndices_extantion<has_indices> {
	public :
		unsigned int VAO, VBO;

		typedef geometry::Face<num_t, Size, is_colored, has_noramls, is_textured, has_indices> ShapeType;
		using point_t = ShapeType::point_t;

		ShapeLinker (unsigned int VAO = 0, unsigned int VBO = 0, unsigned int EBO = 0					) requires( has_indices) : VAO(VAO), VBO(VBO) { this->EBO = EBO;						  }
		ShapeLinker (std::initializer_list<num_t> init, std::vector<unsigned int> indices) requires( has_indices)					  { build(std::vector<num_t>(init), indices); }
		ShapeLinker (std::vector<num_t> vertices, std::vector<unsigned int> indices	  ) requires( has_indices)					  { build(vertices, indices);				 }
		ShapeLinker (unsigned int VAO = 0, unsigned int VBO = 0								  ) requires(!has_indices) :VAO(VAO), VBO(VBO)  {										   }
		ShapeLinker (std::initializer_list<num_t> init						   )											 { build(std::vector<num_t>(init));		  }
		ShapeLinker (std::vector<num_t> vertices								 )											 { build(vertices);						  }

		~ShapeLinker() {
			if constexpr(is_textured) glDeleteVertexArrays(1, &this->EBO);
			glDeleteVertexArrays(1, &VAO	  );
			glDeleteBuffers(1, &VBO	  );
		}

		inline void show(int draw_setting = GL_TRIANGLES, int first = 0, int count = 36) {
			glBindVertexArray(VAO);
			if constexpr(has_indices) glDrawElements(draw_setting, count, GL_UNSIGNED_INT, (void*)0);
			else glDrawArrays(draw_setting, first, count);
			glBindVertexArray(0);
		}

	protected :
		inline void build(
			std::vector<geometry::Point<num_t, Size>> points			   ,
			std::vector<geometry::Point<num_t, Size>> noramls	 = nullptr,
			std::vector<geometry::Point<num_t, 4   >> colors	  = nullptr,
			std::vector<geometry::Point<num_t, 2   >> texturcoord = nullptr,
			std::vector<unsigned int> indices = nullptr
		) {
			Arrays::Array<num_t> vertices;
			for (size_t i = 0; i < points.size(); i++) {
				vertices.add(points[i].data);
				if constexpr (has_noramls) if (noramls	) vertices.add(noramls	[i].data);
				if constexpr (is_colored)  if (colors	 ) vertices.add(colors	 [i].data);
				if constexpr (is_textured) if (texturcoord) vertices.add(texturcoord[i].data);
			} build(vertices, indices);
		}
	inline void build(std::vector<num_t> vertices, std::vector<unsigned int> indices = {}) {
			point_t pos;
			int point_row_size = int(sizeof(pos.data) / sizeof(num_t)), setted = 0;
			if constexpr(has_noramls) point_row_size *= 2;
			if constexpr(is_textured) point_row_size += 2;
			if constexpr(is_colored ) point_row_size += 4;
			glGenVertexArrays(1, &VAO);
			glGenBuffers	 (1, &VBO);

			glBindVertexArray(VAO);

			glBindBuffer(GL_ARRAY_BUFFER, VBO															 );
			glBufferData(GL_ARRAY_BUFFER, sizeof(num_t) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

			if constexpr(has_indices) {
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO													);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);
			}

			glVertexAttribPointer	( 0, Size, GL_FLOAT, GL_FALSE, point_row_size * sizeof(num_t), reinterpret_cast<void*>(static_cast<uintptr_t>(setted)) );
			glEnableVertexAttribArray(0);
			setted = Size * sizeof(num_t);

			if constexpr(has_noramls) {
				glVertexAttribPointer( 1, Size, GL_FLOAT, GL_FALSE, point_row_size * sizeof(num_t), reinterpret_cast<void*>(static_cast<uintptr_t>(setted)) );
				glEnableVertexAttribArray(2);
				setted *= 2;
			}

			if constexpr(is_textured) {
				glVertexAttribPointer( 1, 2   , GL_FLOAT, GL_FALSE, point_row_size * sizeof(num_t), reinterpret_cast<void*>(static_cast<uintptr_t>(setted)) );
				glEnableVertexAttribArray(3);
				setted += 2*sizeof(num_t);
			}

			if constexpr(is_colored) {
				glVertexAttribPointer( 1, 4   , GL_FLOAT, GL_FALSE, point_row_size * sizeof(num_t), reinterpret_cast<void*>(static_cast<uintptr_t>(setted)) );
				glEnableVertexAttribArray(1);
				setted += 4 * sizeof(num_t);
			}
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
		}
	};



	template<typename num_t> requires (std::is_arithmetic_v<num_t>) using FaceLinker2D	  = ShapeLinker<num_t, 2, false, false, false, false>;
	template<typename num_t> requires (std::is_arithmetic_v<num_t>) using FaceLinker3D	  = ShapeLinker<num_t, 3, false, false, false, false>;
	template<typename num_t> requires (std::is_arithmetic_v<num_t>) using FaceLinker4D	  = ShapeLinker<num_t, 4, false, false, false, false>;
	template<typename num_t> requires (std::is_arithmetic_v<num_t>) using TFaceLinker2D	 = ShapeLinker<num_t, 2, false, false, true , false>;
	template<typename num_t> requires (std::is_arithmetic_v<num_t>) using TFaceLinker3D	 = ShapeLinker<num_t, 3, false, false, true , false>;
	template<typename num_t> requires (std::is_arithmetic_v<num_t>) using TFaceLinker4D	 = ShapeLinker<num_t, 4, false, false, true , false>;
	template<typename num_t> requires (std::is_arithmetic_v<num_t>) using CFaceLinker2D	 = ShapeLinker<num_t, 2, true , false, false, false>;
	template<typename num_t> requires (std::is_arithmetic_v<num_t>) using CFaceLinker3D	 = ShapeLinker<num_t, 3, true , false, false, false>;
	template<typename num_t> requires (std::is_arithmetic_v<num_t>) using CFaceLinker4D	 = ShapeLinker<num_t, 4, true , false, false, false>;
	template<typename num_t> requires (std::is_arithmetic_v<num_t>) using CTFaceLinker2D	= ShapeLinker<num_t, 2, true , false, true , false>;
	template<typename num_t> requires (std::is_arithmetic_v<num_t>) using CTFaceLinker3D	= ShapeLinker<num_t, 3, true , false, true , false>;
	template<typename num_t> requires (std::is_arithmetic_v<num_t>) using CTFaceLinker4D	= ShapeLinker<num_t, 4, true , false, true , false>;
	template<typename num_t> requires (std::is_arithmetic_v<num_t>) using PolygonLinker2D   = ShapeLinker<num_t, 2, false, true , false, false>;
	template<typename num_t> requires (std::is_arithmetic_v<num_t>) using PolygonLinker3D   = ShapeLinker<num_t, 3, false, true , false, false>;
	template<typename num_t> requires (std::is_arithmetic_v<num_t>) using PolygonLinker4D   = ShapeLinker<num_t, 4, false, true , false, false>;
	template<typename num_t> requires (std::is_arithmetic_v<num_t>) using CPolygonLinker2D  = ShapeLinker<num_t, 2, true , true , false, false>;
	template<typename num_t> requires (std::is_arithmetic_v<num_t>) using CPolygonLinker3D  = ShapeLinker<num_t, 3, true , true , false, false>;
	template<typename num_t> requires (std::is_arithmetic_v<num_t>) using CPolygonLinker4D  = ShapeLinker<num_t, 4, true , true , false, false>;
	template<typename num_t> requires (std::is_arithmetic_v<num_t>) using ShapeLinker2D	 = ShapeLinker<num_t, 2, false, false, false, true >;
	template<typename num_t> requires (std::is_arithmetic_v<num_t>) using ShapeLinker3D	 = ShapeLinker<num_t, 3, false, false, false, true >;
	template<typename num_t> requires (std::is_arithmetic_v<num_t>) using ShapeLinker4D	 = ShapeLinker<num_t, 4, false, false, false, true >;
	template<typename num_t> requires (std::is_arithmetic_v<num_t>) using CShapeLinker2D	= ShapeLinker<num_t, 2, true , false, false, true >;
	template<typename num_t> requires (std::is_arithmetic_v<num_t>) using CShapeLinker3D	= ShapeLinker<num_t, 3, true , false, false, true >;
	template<typename num_t> requires (std::is_arithmetic_v<num_t>) using CShapeLinker4D	= ShapeLinker<num_t, 4, true , false, false, true >;
	template<typename num_t> requires (std::is_arithmetic_v<num_t>) using CTShapeLinker2D   = ShapeLinker<num_t, 2, true , false, true , true >;
	template<typename num_t> requires (std::is_arithmetic_v<num_t>) using CTShapeLinker3D   = ShapeLinker<num_t, 3, true , false, true , true >;
	template<typename num_t> requires (std::is_arithmetic_v<num_t>) using CTShapeLinker4D   = ShapeLinker<num_t, 4, true , false, true , true >;
	template<typename num_t> requires (std::is_arithmetic_v<num_t>) using PolyShapeLinker2D = ShapeLinker<num_t, 2, false, true , false, true >;

	template<typename num_t> requires (std::is_arithmetic_v<num_t>) using PolyShapeLinker3D = ShapeLinker<num_t, 3, false, true , false, true >;
	template<typename num_t> requires (std::is_arithmetic_v<num_t>) using PolyShapeLinker4D = ShapeLinker<num_t, 4, false, true , false, true >;
	template<typename num_t> requires (std::is_arithmetic_v<num_t>) using ModelLinker2D	 = ShapeLinker<num_t, 2, true , true , false, true >;
	template<typename num_t> requires (std::is_arithmetic_v<num_t>) using ModelLinker3D	 = ShapeLinker<num_t, 3, true , true , false, true >;
	template<typename num_t> requires (std::is_arithmetic_v<num_t>) using ModelLinker4D	 = ShapeLinker<num_t, 4, true , true , false, true >;
	template<typename num_t> requires (std::is_arithmetic_v<num_t>) using MeshLinker2D	  = ShapeLinker<num_t, 2, true , true , true , true >;
	template<typename num_t> requires (std::is_arithmetic_v<num_t>) using MeshLinker3D	  = ShapeLinker<num_t, 3, true , true , true , true >;
	template<typename num_t> requires (std::is_arithmetic_v<num_t>) using MeshLinker4D	  = ShapeLinker<num_t, 4, true , true , true , true >;
	using FaceLinkerF2D	  = ShapeLinker<float, 2, false, false, false, false>;
	using FaceLinkerF3D	  = ShapeLinker<float, 3, false, false, false, false>;
	using FaceLinkerF4D	  = ShapeLinker<float, 4, false, false, false, false>;
	using TFaceLinkerF2D	 = ShapeLinker<float, 2, false, false, true , false>;
	using TFaceLinkerF3D	 = ShapeLinker<float, 3, false, false, true , false>;
	using TFaceLinkerF4D	 = ShapeLinker<float, 4, false, false, true , false>;
	using CFaceLinkerF2D	 = ShapeLinker<float, 2, true , false, false, false>;
	using CFaceLinkerF3D	 = ShapeLinker<float, 3, true , false, false, false>;
	using CFaceLinkerF4D	 = ShapeLinker<float, 4, true , false, false, false>;
	using CTFaceLinkerF2D	= ShapeLinker<float, 2, true , false, true , false>;
	using CTFaceLinkerF3D	= ShapeLinker<float, 3, true , false, true , false>;
	using CTFaceLinkerF4D	= ShapeLinker<float, 4, true , false, true , false>;
	using PolygonLinkerF2D   = ShapeLinker<float, 2, false, true , false, false>;
	using PolygonLinkerF3D   = ShapeLinker<float, 3, false, true , false, false>;
	using PolygonLinkerF4D   = ShapeLinker<float, 4, false, true , false, false>;
	using CPolygonLinkerF2D  = ShapeLinker<float, 2, true , true , false, false>;
	using CPolygonLinkerF3D  = ShapeLinker<float, 3, true , true , false, false>;
	using CPolygonLinkerF4D  = ShapeLinker<float, 4, true , true , false, false>;
	using ShapeLinkerF2D	 = ShapeLinker<float, 2, false, false, false, true >;
	using ShapeLinkerF3D	 = ShapeLinker<float, 3, false, false, false, true >;
	using ShapeLinkerF4D	 = ShapeLinker<float, 4, false, false, false, true >;
	using CShapeLinkerF2D	= ShapeLinker<float, 2, true , false, false, true >;
	using CShapeLinkerF3D	= ShapeLinker<float, 3, true , false, false, true >;
	using CShapeLinkerF4D	= ShapeLinker<float, 4, true , false, false, true >;
	using CTShapeLinkerF2D   = ShapeLinker<float, 2, true , false, true , true >;
	using CTShapeLinkerF3D   = ShapeLinker<float, 3, true , false, true , true >;
	using CTShapeLinkerF4D   = ShapeLinker<float, 4, true , false, true , true >;
	using PolyShapeLinkerF2D = ShapeLinker<float, 2, false, true , false, true >;
	using PolyShapeLinkerF3D = ShapeLinker<float, 3, false, true , false, true >;
	using PolyShapeLinkerF4D = ShapeLinker<float, 4, false, true , false, true >;
	using ModelLinkerF2D	 = ShapeLinker<float, 2, true , true , false, true >;
	using ModelLinkerF3D	 = ShapeLinker<float, 3, true , true , false, true >;
	using ModelLinkerF4D	 = ShapeLinker<float, 4, true , true , false, true >;
	using MeshLinkerF2D	  = ShapeLinker<float, 2, true , true , true , true >;
	using MeshLinkerF3D	  = ShapeLinker<float, 3, true , true , true , true >;
	using MeshLinkerF4D	  = ShapeLinker<float, 4, true , true , true , true >;



	enum class GraphicEngine {
		OpenGL= 0, Vulkan= 1//, CUDA= 2
	};

	class BasicGraph {
	public :
		byte GE : 1 = 0;
		inline void set_line_size(float size = 1.0f) { glLineWidth(size); }

		inline void set_view_rect(geometry::Rect2D<int> r) { glViewport(r.x, r.y, r.width, r.height); }
		inline void set_view_rect(int x, int y, int w, int h) { glViewport(x, y, w, h); }

		inline void clear_color(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f) { glClearColor(r, g, b, a); }
		inline void clear_color(ColorF color) { glClearColor(color.r, color.g, color.b, color.a); }

		inline void clear(unsigned int mask = 0x00004000 | 0x00000100) { glClear(mask); }
		inline void clear(float r, float g, float b, float a = 1.0f, unsigned int mask = 0x00004000 | 0x00000100) {
			clear_color(r, g, b, a);
			glClear	(mask);
		}
		inline void clear(ColorF color,	unsigned int mask = 0x00004000 | 0x00000100) {
			clear_color(color.r, color.g, color.b, color.a);
			glClear(mask);
		}
		inline void clear(RGBColorF color, unsigned int mask = 0x00004000 | 0x00000100) {
			clear_color(color.r, color.g, color.b);
			glClear(mask);
		}


		inline void active_depth(int depth_t = 0x0B71, int blend_t = 0x0BE2, int src_t = 0x0302, int OMS_t = 0x0303) {
			glEnable(depth_t);
			glEnable(blend_t);
			glBlendFunc(src_t, OMS_t);
		}

		inline void use(Shader shaderProgram) { shaderProgram.pack(); }
		inline void use_shader(unsigned int ID)  { glUseProgram(ID); }
	};


	class CPUGraph {
	public :
		byte GE : 1 = 0;

		template<typename num_t = float>
		requires (std::is_arithmetic_v<num_t>)
		inline void set_color(Color<num_t> color)
			{ this->set_color<num_t>(color.r, color.g, color.b, color.a); }

		template<typename num_t = float>
		requires (std::is_arithmetic_v<num_t>)
		inline void set_color(num_t r, num_t g, num_t b, num_t a = 1.0f) {
			if ((r * g * b * a) < 0) return;
			if constexpr (std::is_same_v<num_t, float>) {
				if (a == 1.0f)
					glColor3f(float(r), float(g), float(b));
				else
					glColor4f(float(r), float(g), float(b), float(a));
			} else if constexpr (std::is_same_v<num_t, double>) {
				if (a == 1.0)
					glColor3d(double(r), double(g), double(b));
				else
					glColor4d(double(r), double(g), double(b), double(a));
			} else if constexpr (std::is_same_v<num_t, short>) {
				if (a == 255)
					glColor3s(short(r), short(g), short(b));
				else
					glColor4s(short(r), short(g), short(b), short(a));
			} else if constexpr (std::is_same_v<num_t, int>) {
				if (a == 255)
					glColor3i(int(r), int(g), int(b));
				else
					glColor4i(int(r), int(g), int(b), int(a));
			} else if constexpr (std::is_same_v<num_t, unsigned int>) {
				if (a == 255)
					glColor3ui((unsigned int)(r), (unsigned int)(g), (unsigned int)(b));
				else
					glColor4ui((unsigned int)(r), (unsigned int)(g), (unsigned int)(b), (unsigned int)(a));
			} else if constexpr (std::is_same_v<num_t, GLbyte>) {
				if (a == GLbyte(255))
					glColor3b(GLbyte(r), GLbyte(g), GLbyte(b));
				else
					glColor4b(GLbyte(r), GLbyte(g), GLbyte(b), GLbyte(a));
			} else
					glColor4f(float(r), float(g), float(b), float(a));
		}



		template<typename num_t = float>
		requires (std::is_arithmetic_v<num_t>)
		inline void draw_line2d(
				geometry::Line2D<num_t> line,
				Color<num_t> CA = Color<num_t>(-1,1,1,1),
				Color<num_t> CB = Color<num_t>(-1,1,1,1)) {
			glBegin(GL_LINES);
				this->set_color(CA);
				this->set_point2d<num_t>(line.posA);
				this->set_color(CB);
				this->set_point2d<num_t>(line.posB);
			glEnd();
		}

		

		template<typename num_t = float>
		requires (std::is_arithmetic_v<num_t>)
		inline void draw_line3d(
				geometry::Line3D<num_t> line,
				Color<num_t> CA = Color<num_t>(-1,1,1,1),
				Color<num_t> CB = Color<num_t>(-1,1,1,1)) {
			glBegin(GL_LINES);
				this->set_color(CA);
				this->set_point3d<num_t>(line.posA);
				this->set_color(CB);
				this->set_point3d<num_t>(line.posB);
			glEnd();
		}


		template<typename num_t = float>
		requires (std::is_arithmetic_v<num_t>)
		inline void draw_point2d(num_t x, num_t y, Color<num_t> color = Color<num_t>(-1,1,1,1)) {
			glBegin(GL_POINTS);
				this->set_color(color);
				this->set_point2d(x, y);
			glEnd();
		}
		template<typename num_t = float>
		requires (std::is_arithmetic_v<num_t>)
		inline void draw_point3d(num_t x, num_t y, num_t z, Color<num_t> color = Color<num_t>(-1,1,1,1)) {
			glBegin(GL_POINTS);
				this->set_color<num_t>(color);
				this->set_point3d<num_t>(x, y, z);
			glEnd();
		}

		// set points 2 and 3 d
		template<typename num_t = float>
		requires (std::is_arithmetic_v<num_t>)
		inline void set_point2d(geometry::Point2D<num_t> p)
			{ set_point2d(p.x, p.y); }
		template<typename num_t = float>
		requires (std::is_arithmetic_v<num_t>)
		inline void set_point2d(num_t* p)
			{ set_point2d(p[0], p[1]); }
		template<typename num_t = float>
		requires (std::is_arithmetic_v<num_t>)
		inline void set_point2d(num_t x, num_t y) {
			if constexpr (std::is_same_v<num_t, float>) glVertex2f(float(x), float(y)  );
			else if constexpr (std::is_same_v<num_t, double>) glVertex2d(double(x), double(y));
			else if constexpr (std::is_same_v<num_t, short>) glVertex2s(short(x), short(y)  );
			else if constexpr (std::is_same_v<num_t, int>) glVertex2i(int(x), int(y)	  );
			else glVertex2f(float(x), float(y));
		}

		template<typename num_t = float> requires (std::is_arithmetic_v<num_t>)
		inline void set_point3d(num_t* p)
			{ set_point3d(p[0], p[1], p[2]); }
		template<typename num_t = float> requires (std::is_arithmetic_v<num_t>)
		inline void set_point3d(geometry::Point3D<num_t> p)
			{ set_point3d(p.x, p.y, p.z); }
		template<typename num_t = float>
		requires (std::is_arithmetic_v<num_t>)
		inline void set_point3d(const num_t& x, const num_t& y, const num_t& z) {
			if	  constexpr (std::is_same_v<num_t, float> ) glVertex3f(float(x),  float(y),  float(z));
			else if constexpr (std::is_same_v<num_t, double>) glVertex3d(double(x), double(y), float(z));
			else if constexpr (std::is_same_v<num_t, short> ) glVertex3s(short(x),  short(y),  float(z));
			else if constexpr (std::is_same_v<num_t, int>   ) glVertex3i(int(x),	int(y),	float(z));
			else glVertex3f(float(x), float(y), float(z));
		}


		/* show circles:
			* fill/draw circle 2/3 d
		*/
		template<typename num_t, unsigned int algs_num = 360, unsigned int step = 1>
		requires (std::is_arithmetic_v<num_t>)
		inline void fill_circle2d(geometry::Line2D<num_t> line, Color<num_t> color = {})
			{ this->show_circle2d<0x0006, num_t, algs_num, step>(line, color); }

		template<typename num_t, unsigned int algs_num = 360, unsigned int step = 1>
		requires (std::is_arithmetic_v<num_t>)
		inline void draw_circle2d(geometry::Line2D<num_t> line, Color<num_t> color = {})
			{ this->show_circle2d<0x0002,num_t, algs_num, step>(line, color); }

		template<int setting, typename num_t, unsigned int algs_num = 360, unsigned int step = 1>
		requires (std::is_arithmetic_v<num_t>)
		inline void show_circle2d(geometry::Line2D<num_t> line, Color<num_t> color = {}) {
			this->set_color<num_t>(color);
			glBegin(setting); for (size_t i = 0; i < algs_num; i += step) {
				float angle = float(i * 3.141592 / 180);
				this->set_point2d(
					num_t(line.posA.x + line.posB.x * std::cos(angle)),
					num_t(line.posA.y + line.posB.y * std::sin(angle)));
			} glEnd();
		}

		template<typename num_t, unsigned int algs_num = 360, unsigned int step = 1>
		requires (std::is_arithmetic_v<num_t>)
		inline void fill_circle3d(geometry::Line3D<num_t> line, Color<num_t> color = {})
			{ this->show_circle3d<0x0006, num_t, algs_num, step>(line, color); }

		template<typename num_t, unsigned int algs_num = 360, unsigned int step = 1>
		requires (std::is_arithmetic_v<num_t>)
		inline void draw_circle3d(geometry::Line3D<num_t> line, Color<num_t> color = {})
			{ this->show_circle3d<0x0002,num_t, algs_num, step>(line, color); }

		template<int setting, typename num_t, unsigned int algs_num = 360, unsigned int step = 1>
		requires (std::is_arithmetic_v<num_t>)
		inline void show_circle3d(geometry::Line3D<num_t> line, Color<num_t> color = {}) {
			this->set_color<num_t>(color);
			glBegin(setting); for (size_t i = 0; i < algs_num; i += step) {
				float angle = float(i * 3.141592 / 180);
				this->set_point3d(
					num_t(line.posA.x + line.posB.x * std::cos(angle)),
					num_t(line.posA.y + line.posB.y * std::sin(angle)),
					num_t(line.posA.z + line.posB.z * angle));
			} glEnd();
		}


		template<typename num_t = float, bool pin_points = false>
		requires (std::is_arithmetic_v<num_t>)
		inline void draw_rect2d(geometry::Line2D<num_t> r, Color<num_t> color = { -1,1,1,1 })
			{ this->show_rect2d<0x0002, num_t, pin_points>(r, color); }

		template<typename num_t = float, bool pin_points = false>
		requires (std::is_arithmetic_v<num_t>)
		inline void fill_rect2d(geometry::Line2D<num_t> r, Color<num_t> color = { -1,1,1,1 })
			{ this->show_rect2d<0x0007, num_t, pin_points>(r, color); }

		template<int setting, typename num_t = float, bool pin_points = false>
		requires (std::is_arithmetic_v<num_t>)
		inline void show_rect2d(geometry::Line2D<num_t> r, Color<num_t> color = { -1,1,1,1 }) {
			num_t xw, yh;
			if constexpr (pin_points) {
				xw = r[2];
				yh = r[3];
			} else {
				xw = r[0] + r[2];
				yh = r[1] + r[3];
			}
			this->set_color<num_t>(color);
			glBegin(setting);
				this->set_point2d<num_t>(r[0], r[1]);
				this->set_point2d<num_t>(xw  , r[1]);
				this->set_point2d<num_t>(xw  , yh  );
				this->set_point2d<num_t>(r[0], yh  );
			glEnd();
		}

		template<typename num_t = float, bool pin_points = false>
		requires (std::is_arithmetic_v<num_t>)
		inline void draw_rect2d(geometry::Line2D<num_t> r, Color<num_t> colors[4])
			{ this->show_rect2d<0x0002, num_t, pin_points>(r, colors); }

		template<typename num_t = float, bool pin_points = false>
		requires (std::is_arithmetic_v<num_t>)
		inline void fill_rect2d(geometry::Line2D<num_t> r, Color<num_t> colors[4])
			{ this->show_rect2d<0x0007, num_t, pin_points>(r, colors); }

		template<int setting, typename num_t = float, bool pin_points = false>
		requires (std::is_arithmetic_v<num_t>)
		inline void show_rect2d(geometry::Line2D<num_t> r, Color<num_t> colors[4]) {
			num_t xw, yh;
			if constexpr (pin_points) {
				xw = r[2];
				yh = r[3];
			} else {
				xw = r[0] + r[2];
				yh = r[1] + r[3];
			}
			glBegin(setting);
				this->set_color<num_t>(colors[0]);
				this->set_point2d<num_t>(r[0], r[1]);
				this->set_color<num_t>(colors[1]);
				this->set_point2d<num_t>(xw  , r[1]);
				this->set_color<num_t>(colors[2]);
				this->set_point2d<num_t>(xw  , yh);
				this->set_color<num_t>(colors[3]);
				this->set_point2d<num_t>(r[0], yh);
			glEnd();
		}



		template<typename num_t = float, bool pin_points = false>
		requires (std::is_arithmetic_v<num_t>)
		inline void draw_rect3d(geometry::Line3D<num_t> r, Color<num_t> color = { -1,1,1,1 })
			{ this->show_rect3d<0x0002, num_t, pin_points>(r, color); }

		template<typename num_t = float, bool pin_points = false>
		requires (std::is_arithmetic_v<num_t>)
		inline void fill_rect3d(geometry::Line3D<num_t> r, Color<num_t> color = { -1,1,1,1 })
			{ this->show_rect3d<0x0007, num_t, pin_points>(r, color); }

		template<int setting, typename num_t = float, bool pin_points = false>
		requires (std::is_arithmetic_v<num_t>)
		inline void show_rect3d(geometry::Line3D<num_t> r, Color<num_t> color = { -1,1,1,1 }) {
			num_t xw, yh, zl;
			if constexpr (pin_points) {
				xw = r[3];
				yh = r[4];
				zl = r[5];
			} else {
				xw = r[0] + r[3];
				yh = r[1] + r[4];
				zl = r[2] + r[5];
			}
			this->set_color<num_t>(color);
			glBegin(setting);
				this->set_point3d<num_t>(r.x, r.y, r.z);
				this->set_point3d<num_t>(xw , r.y, r.z);
				this->set_point3d<num_t>(xw , yh , r.z);
				this->set_point3d<num_t>(r.x, yh , r.z);
				this->set_point3d<num_t>(r.x, yh , zl );
				this->set_point3d<num_t>(r.x, r.y, zl );
				this->set_point3d<num_t>(xw , r.y, zl );
				this->set_point3d<num_t>(xw , yh , zl );
			glEnd();
		}

		template<typename num_t = float, bool pin_points = false>
		requires (std::is_arithmetic_v<num_t>)
		inline void draw_rect3d(geometry::Line3D<num_t> r, Color<num_t> colors[8] = {})
			{ this->show_rect3d<0x0002, num_t, pin_points>(r, colors); }

		template<typename num_t = float, bool pin_points = false>
		requires (std::is_arithmetic_v<num_t>)
		inline void fill_rect3d(geometry::Line3D<num_t> r, Color<num_t> colors[8] = {})
			{ this->show_rect3d<0x0007, num_t, pin_points>(r, colors); }

		template<int setting, typename num_t = float, bool pin_points = false>
		requires (std::is_arithmetic_v<num_t>)
		inline void show_rect3d(geometry::Line3D<num_t> r, Color<num_t> colors[8] = {}) {
			num_t xw, yh, zl;
			if constexpr (pin_points) {
				xw = r[3];
				yh = r[4];
				zl = r[5];
			} else {
				xw = r[0] + r[3];
				yh = r[1] + r[4];
				zl = r[2] + r[5];
			}
			glBegin(setting);
				this->set_color<num_t>(colors[0]);
				this->set_point3d<num_t>(r.x, r.y, r.z);
				this->set_color<num_t>(colors[1]);
				this->set_point3d<num_t>(xw , r.y, r.z);
				this->set_color<num_t>(colors[2]);
				this->set_point3d<num_t>(xw , yh , r.z);
				this->set_color<num_t>(colors[3]);
				this->set_point3d<num_t>(r.x, yh , r.z);
				this->set_color<num_t>(colors[4]);
				this->set_point3d<num_t>(r.x, yh , zl );
				this->set_color<num_t>(colors[5]);
				this->set_point3d<num_t>(r.x, r.y, zl );
				this->set_color<num_t>(colors[6]);
				this->set_point3d<num_t>(xw , r.y, zl );
				this->set_color<num_t>(colors[7]);
				this->set_point3d<num_t>(xw , yh , zl );
			glEnd();
		}


		template<typename num_t = float> requires (std::is_arithmetic_v<num_t>) inline void fill(geometry::Face3D<num_t> face, std::vector<Color<num_t>> colors = {}) { this->show<0x0004, num_t>(face, colors); }
		template<typename num_t = float> requires (std::is_arithmetic_v<num_t>) inline void draw(geometry::Face3D<num_t> face, std::vector<Color<num_t>> colors = {}) { this->show<0x0002, num_t>(face, colors); }
		template<int setting, typename num_t = float> requires (std::is_arithmetic_v<num_t>) inline void show(geometry::Face3D<num_t> face, std::vector<Color<num_t>> colors = {}) {
			glBegin(setting);
				for (size_t i = 0, size = colors.size(); i < face.points.size(); i++) {
					if (i < size) this->set_color<num_t>(colors[i]);
					this->set_point3d<num_t>(face[i][0], face[i][1], face[i][2]);
				}
			glEnd();
		}

		template<typename num_t = float> requires (std::is_arithmetic_v<num_t>) inline void fill(std::vector<num_t> vx, std::vector<num_t> vy, std::vector<num_t> vz, std::vector<Color<num_t>> colors = {}) { this->show<0x0004, num_t>(vx, vy, vz, colors); }
		template<typename num_t = float> requires (std::is_arithmetic_v<num_t>) inline void draw(std::vector<num_t> vx, std::vector<num_t> vy, std::vector<num_t> vz, std::vector<Color<num_t>> colors = {}) { this->show<0x0002, num_t>(vx, vy, vz, colors); }
		template<int setting, typename num_t = float> requires (std::is_arithmetic_v<num_t>) inline void show(std::vector<num_t> vx, std::vector<num_t> vy, std::vector<num_t> vz, std::vector<Color<num_t>> colors = {}) {
			glBegin(setting); for (size_t i = 0, size = colors.size(); i < vx.size(); i++) {
				if (i < size) this->set_color<num_t>(colors[i]);
				this->set_point3d<num_t>(vx[i], vy[i], vz[i]);
			} glEnd();
		}

		inline void new_layer(GLFWwindow* wind_ptr, int width, int height) { glfwGetFramebufferSize(wind_ptr, &width, &height); }

		inline static void active_2d(int mode_0 = GL_PROJECTION, int mode_1 = GL_MODELVIEW) {
			glUseProgram(0);
			glMatrixMode(mode_0);
			glLoadIdentity();
			glMatrixMode(mode_1);
			glLoadIdentity();
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}



		// draw a graid
		template<bool drwOsr = false, bool pinCrds = true, typename num_t = float>
		requires (std::is_arithmetic_v<num_t>)
		inline void draw_graid2d(geometry::Rect2D<num_t> r, Color<num_t> color = {-1,1,1,1}) {
			for (geometry::Point2D<num_t> i = {r.x, r.x}; i.x <= r.y and i.y <= r.y ; i += geometry::Point2D<num_t>(r.xb, r.yb)) {
				draw_line2d<num_t>(r.x, i.y, r.y, i.y, color);
				draw_line2d<num_t>(i.x, r.x, i.x, r.y, color);
			}
			if constexpr (drwOsr)
				draw_rect2d<pinCrds, num_t>(r.x, r.x, r.y, r.y, color);
		}
		template<bool drwOsr = false, bool pinCrds = true, typename num_t = float>
		requires (std::is_arithmetic_v<num_t>)
		inline void draw_graid(geometry::Rect2D<num_t> r, geometry::Point2D<num_t> ri, Color<num_t> color = {-1,1,1,1}) {
			for (num_t i = r.y; i <= r.yb ; i+=ri.y) draw_line2d<num_t>(r.x, i, r.xb, i, color);
			for (num_t i = r.x; i <= r.xb ; i+=ri.x) draw_line2d<num_t>(i, r.y, i, r.yb, color);
			if constexpr (drwOsr)
				draw_rect2d<pinCrds, num_t>(r.x, r.y, r.xb, r.yb, color, pinCrds);
		}
		template<bool drwOsr = false, bool pinCrds = true, typename num_t = float>
		requires (std::is_arithmetic_v<num_t>)
		inline void draw_graid2d(geometry::Rect2D<num_t> r, Color<num_t> colors[4]) {
			for (geometry::Point2D<num_t> i = {r.x, r.x}; i.x <= r.y and i.y <= r.y ; i += geometry::Point2D<num_t>(r.xb, r.yb)) {
				draw_line2d<num_t>(r.x, i.y, r.y, i.y, colors[0], colors[1]);
				draw_line2d<num_t>(i.x, r.x, i.x, r.y, colors[2], colors[3]);
			}
			if constexpr (drwOsr)
				draw_rect2d<pinCrds, num_t>(r.x, r.x, r.y, r.y, colors);
		}
		template<bool drwOsr = false, bool pinCrds = true, typename num_t = float>
		requires (std::is_arithmetic_v<num_t>)
		inline void draw_graid(geometry::Rect2D<num_t> r, geometry::Point2D<num_t> ri, Color<num_t> colors[4]) {
			for (num_t i = r.y; i <= r.yb ; i+=ri.y) draw_line2d<num_t>(r.x, i, r.xb, i, colors[0], colors[1]);
			for (num_t i = r.x; i <= r.xb ; i+=ri.x) draw_line2d<num_t>(i, r.y, i, r.yb, colors[2], colors[3]);
			if constexpr (drwOsr)
				draw_rect2d<pinCrds, num_t>(r.x, r.y, r.xb, r.yb, colors);
		}



		template<bool is_lookedin = true, typename num_t = float>
		requires (std::is_arithmetic_v<num_t>)
		inline void draw_cursor_cross(geometry::Point2D<num_t> mouse, geometry::Rect2D<num_t> r, Color<num_t> color = {-1, 1, 1}) { if constexpr (is_lookedin) {
			if (mouse.y < r.yb and mouse.y > r.y) draw_line2d<num_t>(r.x, mouse.y, r.xb, mouse.y, color);
			if (mouse.x < r.xb and mouse.x > r.x) draw_line2d<num_t>(mouse.x, r.y, mouse.x, r.yb, color);
		}}
		template<bool is_lookedin = true, typename num_t = float>
		requires (std::is_arithmetic_v<num_t>)
		inline void draw_cursor_cross(geometry::Point2D<num_t> mouse, geometry::Rect2D<num_t> r, Color<num_t> colors[4]) { if constexpr (is_lookedin) {
			if (mouse.y < r.yb and mouse.y > r.y) draw_line2d<num_t>(r.x, mouse.y, r.xb, mouse.y, colors[0], colors[1]);
			if (mouse.x < r.xb and mouse.x > r.x) draw_line2d<num_t>(mouse.x, r.y, mouse.x, r.yb, colors[2], colors[3]);
		}}
		template<bool is_lookedin = true, typename num_t = float>
		requires (std::is_arithmetic_v<num_t>)
		inline void draw_cursor_cross(GLFWwindow* wind, geometry::Rect2D<num_t> r, Color<num_t> color = {-1, 1, 1}) { if constexpr (is_lookedin) {
			Input::Mouse mouse;
			mouse.update_position(wind);
			if (mouse.y < r.yb and mouse.y > r.y) draw_line2d<num_t>(r.x, mouse.y, r.xb, mouse.y, color);
			if (mouse.x < r.xb and mouse.x > r.x) draw_line2d<num_t>(mouse.x, r.y, mouse.x, r.yb, color);
		}}
		template<bool is_lookedin = true, typename num_t = float>
		requires (std::is_arithmetic_v<num_t>)
		inline void draw_cursor_cross(GLFWwindow* wind, geometry::Rect2D<num_t> r, Color<num_t> colors[4]) { if constexpr (is_lookedin) {
			Input::Mouse mouse;
			mouse.update_position(wind);
			if (mouse.y < r.yb and mouse.y > r.y) draw_line2d<num_t>(r.x, mouse.y, r.xb, mouse.y, colors[0], colors[1]);
			if (mouse.x < r.xb and mouse.x > r.x) draw_line2d<num_t>(mouse.x, r.y, mouse.x, r.yb, colors[2], colors[3]);
		}}
	};




	class GPUGraph {
	public :
		byte GE : 1 = 0;

		template<typename num_t = float, int setting = GL_TRIANGLES>
			inline void show3d(std::vector<num_t> verteces)
			{ ShapeLinker<num_t, 3>(verteces).show(setting); }
		template<typename num_t = float, int setting = GL_TRIANGLES>
			inline void show2d(std::vector<num_t> verteces)
			{ ShapeLinker<num_t, 2>(verteces).show(setting); }

		template<typename num_t = float> requires (std::is_arithmetic_v<num_t>)
			inline void draw_line2d(num_t data[6], Color<num_t> CA = Color<num_t>(-1,1,1,1), Color<num_t> CB = Color<num_t>(-1,1,1,1))
			{ draw_line2d<num_t>((geometry::Rect2D<num_t>*)data, CA, CB); }
		template<typename num_t = float> requires (std::is_arithmetic_v<num_t>)
			inline void draw_line2d(geometry::Point2D<num_t> a, geometry::Point2D<num_t> b, Color<num_t> CA = Color<num_t>(-1,1,1,1), Color<num_t> CB = Color<num_t>(-1,1,1,1))
			{ draw_line2d<num_t>(a.x, a.y, b.x, b.y, CA, CB); }
		template<typename num_t = float> requires (std::is_arithmetic_v<num_t>)
			inline void draw_line2d(geometry::Rect2D<num_t> r, Color<num_t> CA = Color<num_t>(-1,1,1,1), Color<num_t> CB = Color<num_t>(-1,1,1,1))
			{ draw_line2d<num_t>(r.x, r.y, r.width, r.height, CA, CB); }
		template<typename num_t = float> requires (std::is_arithmetic_v<num_t>)
			inline void draw_line2d(num_t xa, num_t ya, num_t xb, num_t yb, Color<num_t> CA = Color<num_t>(-1,1,1,1), Color<num_t> CB = Color<num_t>(-1,1,1,1)) {
			num_t cado = CA.template dimopr<'*'>(), cbdo = CB.template dimopr<'*'>();
			if	  (cado > 0 and cbdo < 0) CB = CA;
			else if (cado < 0 and cbdo > 0) CA = CB;
			if (cado < 0 and cbdo < 0) FaceLinker3D<num_t>(std::vector<num_t> {xa, ya, xb, yb}).show(GL_LINES);
			else {
				Arrays::Array<num_t> verteces;
				verteces.add({xa, ya});
				verteces.add(CA.data);
				verteces.add({xb, yb});
				verteces.add(CB.data);
				CFaceLinker3D<num_t>(verteces).show(GL_LINES);
			}
		}

		template<typename num_t = float> requires (std::is_arithmetic_v<num_t>)
			inline void draw_line3d(num_t data[6], Color<num_t> CA = Color<num_t>(-1,1,1,1), Color<num_t> CB = Color<num_t>(-1,1,1,1))
			{ draw_line3d<num_t>((geometry::Rect3D<num_t>*)data, CA, CB); }
		template<typename num_t = float> requires (std::is_arithmetic_v<num_t>)
			inline void draw_line3d(geometry::Point3D<num_t> a, geometry::Point3D<num_t> b, Color<num_t> CA = Color<num_t>(-1,1,1,1), Color<num_t> CB = Color<num_t>(-1,1,1,1))
			{ draw_line3d<num_t>(a.x, a.y, a.z, b.x, b.y, b.z, CA, CB); }
		template<typename num_t = float> requires (std::is_arithmetic_v<num_t>)
			inline void draw_line3d(geometry::Rect3D<num_t> r, Color<num_t> CA = Color<num_t>(-1,1,1,1), Color<num_t> CB = Color<num_t>(-1,1,1,1))
			{ draw_line3d<num_t>(r.x, r.y, r.z, r.width, r.height, r.length, CA, CB); }
		template<typename num_t = float> requires (std::is_arithmetic_v<num_t>)
			inline void draw_line3d(num_t xa, num_t ya, num_t za, num_t xb, num_t yb, num_t zb, Color<num_t> CA = Color<num_t>(-1,1,1,1), Color<num_t> CB = Color<num_t>(-1,1,1,1)) {
			num_t cado = CA.template dimopr<'*'>(), cbdo = CB.template dimopr<'*'>();
			if	  (cado > 0 and cbdo < 0) CB = CA;
			else if (cado < 0 and cbdo > 0) CA = CB;
			if (cado < 0 and cbdo < 0) FaceLinker3D<num_t>(std::vector<num_t>{xa, ya, za, xb, yb, zb}).show(GL_LINES);
			else {
				Arrays::Array<num_t> verteces {
					xa, ya, za,  CA.r, CA.g, CA.b, CA.a,
					xb, yb, zb,  CB.r, CB.g, CB.b, CB.a
				};
				CFaceLinker3D<num_t>(verteces).show(GL_LINES);
			}
		}

		template<typename num_t = float, int setting = GL_TRIANGLES, bool pin_points = false>
			inline void fill_rect2d(num_t x, num_t y, num_t w, num_t h, Color<num_t> colors[4])
			{ show_rect2d<num_t, GL_QUADS, pin_points>(x,y,w,y,colors); }
		template<typename num_t = float, int setting = GL_TRIANGLES, bool pin_points = false>
			inline void draw_rect2d(num_t x, num_t y, num_t w, num_t h, Color<num_t> colors[4])
			{ show_rect2d<num_t, GL_LINE_LOOP, pin_points>(x,y,w,y,colors); }
		template<typename num_t = float, int setting = GL_TRIANGLES, bool pin_points = false>
			inline void show_rect2d(num_t x, num_t y, num_t w, num_t h, Color<num_t> colors[4]) {
			num_t xw, yh;
			if constexpr (pin_points) {
				xw = w;
				yh = h;
			} else {
				xw = x + w;
				yh = y + h;
			} Arrays::Array<num_t> verteces;
			if (colors) {
				verteces.add(colors[0].data);
				verteces.add({x ,y });
				verteces.add(colors[1].data);
				verteces.add({xw,y });
				verteces.add(colors[2].data);
				verteces.add({xw,yh});
				verteces.add(colors[3].data);
				verteces.add({x ,yh});
				CFaceLinker2D<num_t>(verteces).show(setting);
			} else {
				verteces.add({x,y,xw,y,xw,yh,x,yh});
				FaceLinker2D<num_t>(verteces).show(setting);
			}
		}

		template<typename num_t = float, int setting = GL_TRIANGLES, bool pin_points = false>
			inline void fill_rect3d(num_t x, num_t y, num_t z, num_t w, num_t h, num_t l, Color<num_t> colors[8])
			{ show_rect3d<num_t, GL_QUADS, pin_points>(x,y,z,w,y,l,colors); }
		template<typename num_t = float, int setting = GL_TRIANGLES, bool pin_points = false>
			inline void draw_rect3d(num_t x, num_t y, num_t z, num_t w, num_t h, num_t l, Color<num_t> colors[8])
			{ show_rect3d<num_t, GL_LINE_LOOP, pin_points>(x,y,z,w,y,l,colors); }
		template<typename num_t = float, int setting = GL_TRIANGLES, bool pin_points = false>
			inline void show_rect3d(num_t x, num_t y, num_t z, num_t w, num_t h, num_t l, Color<num_t> colors[8]) {
			num_t xw, yh, zl;
			if constexpr (pin_points) {
				xw = w;
				yh = h;
				zl = l;
			} else {
				xw = x + w;
				yh = y + h;
				zl = z + l;
			} Arrays::Array<num_t> verteces;
			if (colors) {
				verteces.add(colors[0].data);
				verteces.add({x ,y ,z });
				verteces.add(colors[1].data);
				verteces.add({xw,y ,z });
				verteces.add(colors[2].data);
				verteces.add({xw,yh,z });
				verteces.add(colors[3].data);
				verteces.add({x ,yh,z });
				verteces.add(colors[4].data);
				verteces.add({x ,yh,zl});
				verteces.add(colors[5].data);
				verteces.add({x ,y ,zl});
				verteces.add(colors[6].data);
				verteces.add({xw,y ,zl});
				verteces.add(colors[7].data);
				verteces.add({xw,yh,zl});
				CFaceLinker2D<num_t>(verteces).show(setting);
			} else {
				verteces.add({
					x ,y ,z ,  xw,y ,z ,  xw,yh,z ,  x ,yh,z ,
					x ,yh,zl,  x ,y ,zl,  xw,y ,zl,  xw,yh,zl
				}); FaceLinker2D<num_t>(verteces).show(setting);
			}
		}
	};



	class Graph {
	public :
		union {
			byte GE;
			BasicGraph s;
			CPUGraph cpu;
			GPUGraph gpu;
		};
		Graph(byte GE = 0) :GE(GE) {}
		Graph(GraphicEngine GE) : GE(static_cast<byte>(GE)) {}

		auto operator -> () { return &this->s; }

		inline void set(GraphicEngine GE) { this->GE = static_cast<byte>(GE); }
		auto get() { return this->GE; }
	};
}
