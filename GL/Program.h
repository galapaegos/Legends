#ifndef _GL_Program_h_
#define _GL_Program_h_

#include "stdafx.h"

#include <glm/glm.hpp>

#include "GL/Shader.h"
#include "GL/Texture.h"

class Program {
public:
	Program(const std::string &vertex, const std::string &fragment);

	void create();
	void destroy();

	void bind();
	void unbind();

	void bind_parameter(const std::string &name, Texture &tex);
	void bind_parameter(const std::string &name, const int &value);
	void bind_parameter(const std::string &name, const float &value);

	void set_uniform(const std::string &loc, const int &value);
	void set_uniform(const std::string &loc, const int &value1, const int &value2);
	void set_uniform(const std::string &loc, const int &value1, const int &value2, const int &value3);

	void set_uniform(const std::string &loc, const float &value);
	void set_uniform(const std::string &loc, const float &value1, const float &value2);
	void set_uniform(const std::string &loc, const float &value1, const float &value2, const float &value3);

	void set_uniform(const std::string &loc, const glm::vec3 &value);
	void set_uniform(const std::string &loc, const glm::mat4 &value);

protected:
	inline GLint get_uniform_location(const std::string &loc) { return glGetUniformLocation(program_id, loc.c_str()); }
	inline GLint get_attrib_location(const std::string &loc) { return glGetAttribLocation(program_id, loc.c_str()); }

	std::unique_ptr<FragmentShader> fragment_shader;
	std::unique_ptr<VertexShader> vertex_shader;

	std::string fragment_shader_file;
	std::string vertex_shader_file;

	GLuint program_id;
	bool link_program;

	struct TextureList {
		std::string name;
		Texture value;
	};

	std::vector<TextureList> textures;

	struct IntegerList {
		std::string name;
		int value;
	};

	std::vector<IntegerList> integers;

	struct FloatList {
		std::string name;
		float value;
	};

	std::vector<FloatList> floats;
};

#endif
