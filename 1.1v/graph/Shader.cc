module;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>
#undef __DEPRECATED

export module skbase.graph.shader;

import skbase.base;
import skbase.files;

import std;


export namespace graph {
	class Shader {
	public : uint ID = 0;
		Shader(file::File vertexShaderFile, file::File fragmentShaderFile):
			Shader(vertexShaderFile.txt()->c_str(), fragmentShaderFile.txt()->c_str()) {}
		Shader(const char* vertexShaderSource, const char* fragmentShaderSource)
			{ setup(vertexShaderSource, fragmentShaderSource); }
		Shader(const char* file_path) {
			setup(  file::get_contents(std::string(std::string(file_path) + ".vertex").c_str()).c_str(),
					file::get_contents(std::string(std::string(file_path) + ".frag"  ).c_str()).c_str());
		}
		~Shader() { glDeleteProgram(ID); }

		inline uint get_id_of(const char* context)
			{ return glGetUniformLocation(ID, context); }

		template<typename used_t>
		uint operator() (const char* context, used_t m, int count = 1, byte transpose = byte(0)) {
			uint ret = get_id_of(context);
			if      constexpr (std::is_same_v<used_t, glm::mat4>) glUniformMatrix4fv(ret, count, transpose, glm::value_ptr(m));
			else if constexpr (std::is_same_v<used_t, glm::mat3>) glUniformMatrix3fv(ret, count, transpose, glm::value_ptr(m));
			else if constexpr (std::is_same_v<used_t, glm::mat2>) glUniformMatrix2fv(ret, count, transpose, glm::value_ptr(m));
			else if constexpr (std::is_same_v<used_t, glm::vec4>) glUniform4f	   (ret, m.x, m.y, m.z, m.w                     );
			else if constexpr (std::is_same_v<used_t, glm::vec3>) glUniform3f	   (ret, m.x, m.y, m.z                          );
			else if constexpr (std::is_same_v<used_t, glm::vec2>) glUniform2f	   (ret, m.x, m.y                               );
			else if constexpr (std::is_same_v<used_t, glm::vec1>) glUniform1f	   (ret, m.x                                    );
			else                                                  glUniform1f	   (ret, m                                      );
			return ret;
		}

		float* get_info_of(const char* context) {
			float* ret = new float();
			glGetUniformfv(this->ID, get_id_of(context), ret);
			return ret;
		}
		float* get_info_of(uint location) {
			float* ret = new float();
			glGetUniformfv(this->ID, location, ret);
			return ret;
		}

		inline void pack() { glUseProgram(ID); }
		
		static Shader skbase_default_shader() {
			return Shader(
				file::get_contents("res/shaders/Moduel.vertex").c_str(),
				file::get_contents("res/shaders/Moduel.frag"  ).c_str()
			);
		}

	private :
		inline void setup(const char* vertexShaderSource, const char* fragmentShaderSource) {
			uint vertexShader = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource (vertexShader, 1, &vertexShaderSource, NULL);
			glCompileShader(vertexShader							  );

			int success;
			char infoLog[512];
			glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
			if (!success) {
				glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
				std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
			}

			uint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource (fragmentShader, 1, &fragmentShaderSource, NULL);
			glCompileShader(fragmentShader								);

			glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
			if (!success) {
				glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
				std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
			}

			ID = glCreateProgram();
			glAttachShader(ID, vertexShader);
			glAttachShader(ID, fragmentShader);
			glLinkProgram(ID);

			glGetProgramiv(ID, GL_LINK_STATUS, &success);
			if (!success) {
				glGetProgramInfoLog(ID, 512, NULL, infoLog);
				std::cerr << "\e[1;31mERROR\e[0m::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
			}

			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);
		}
	};
}
