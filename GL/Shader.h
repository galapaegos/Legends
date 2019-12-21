#ifndef _GL_Shader_h_
#define _GL_Shader_h_

#include "stdafx.h"

std::string get_program_log(GLuint object);

class Shader {
public:
	Shader(const GLenum &type);
	
	void load(const std::string &file);

	bool compile();
	void release();

	void attach(const GLuint &program);
	void detach(const GLuint &program);

protected:
	GLenum shader_type;
	GLuint shader_id;
	std::string shader_file;
	std::string shader_contents;
	bool shader_compiled;
};

class VertexShader : public Shader {
public:
	VertexShader();
};

class FragmentShader : public Shader {
public:
	FragmentShader();
};

#endif
