#include "stdafx.h"

#include "GL/Program.h"

#include <glm/gtc/type_ptr.hpp>

Program::Program(const std::string &vert, const std::string &frag) {
	fragment_shader = std::make_unique<FragmentShader>();
	fragment_shader_file = frag;

	vertex_shader = std::make_unique<VertexShader>();
	vertex_shader_file = vert;

	program_id = 0;
	link_program = false;
}

void Program::create() {
	if (program_id == 0) {
		program_id = glCreateProgram();
		CHECK_ERROR();
	}

	vertex_shader->load(vertex_shader_file);
	fragment_shader->load(fragment_shader_file);

	vertex_shader->attach(program_id);
	fragment_shader->attach(program_id);

	if (!vertex_shader->compile())
		return;
	
	if (!fragment_shader->compile())
		return;
}

void Program::destroy() {
	if (program_id) {
		glDeleteProgram(program_id);
		CHECK_ERROR();
	}

	vertex_shader->release();
	fragment_shader->release();
}

void Program::bind() {
	if (!link_program) {
		glLinkProgram(program_id);
		CHECK_ERROR();

		GLint success = 0;
		glGetProgramiv(program_id, GL_LINK_STATUS, &success);
		CHECK_ERROR();

		std::string log = get_program_log(program_id);
		if (success == GL_FALSE) {
			printf("Unable to link programs [%s][%s]\nlinking:%s\n", vertex_shader_file.c_str(), fragment_shader_file.c_str(), log.c_str());
			link_program = false;
			return;
		}
		else
			link_program = true;
	}

	glUseProgram(program_id);
	CHECK_ERROR();

	int texture_id = 0;
	for (int i = 0; i < textures.size(); i++) {
		set_uniform(textures[i].name, texture_id);

		glActiveTexture(GL_TEXTURE0 + texture_id);

		textures[i].value.bind();

		texture_id++;
	}

	for (int i = 0; i < integers.size(); i++) {
		set_uniform(integers[i].name, integers[i].value);
	}

	for (int i = 0; i < floats.size(); i++) {
		set_uniform(floats[i].name, floats[i].value);
	}

	CHECK_ERROR();
}

void Program::unbind() {
	size_t texture_id = textures.size() - 1;
	for (int i = int(texture_id); i >= 0; i--) {
		glActiveTexture(GL_TEXTURE0 + texture_id);
		textures[i].value.unbind();
	}

	glUseProgram(0);
	CHECK_ERROR();
}

void Program::bind_parameter(const std::string &name, Texture &tex) {
	TextureList tl;
	tl.name = name;
	tl.value = tex;

	textures.push_back(tl);
}

void Program::bind_parameter(const std::string &name, const int &value) {
	IntegerList il;
	il.name = name;
	il.value = value;

	integers.push_back(il);
}

void Program::bind_parameter(const std::string &name, const float &value) {
	FloatList fl;
	fl.name = name;
	fl.value = value;

	floats.push_back(fl);
}

void Program::set_uniform(const std::string &loc, const int &value) {
	GLint position = get_uniform_location(loc);
	glUniform1i(position, value);
	CHECK_ERROR();
}

void Program::set_uniform(const std::string &loc, const int &value1, const int &value2) {
	GLint position = get_uniform_location(loc);
	glUniform2i(position, value1, value2);
	CHECK_ERROR();
}

void Program::set_uniform(const std::string &loc, const int &value1, const int &value2, const int &value3) {
	GLint position = get_uniform_location(loc);
	glUniform3i(position, value1, value2, value3);
	CHECK_ERROR();
}

void Program::set_uniform(const std::string &loc, const float &value) {
	GLint position = get_uniform_location(loc);
	glUniform1f(position, value);
	CHECK_ERROR();
}

void Program::set_uniform(const std::string &loc, const float &value1, const float &value2) {
	GLint position = get_uniform_location(loc);
	glUniform2f(position, value1, value2);
	CHECK_ERROR();
}

void Program::set_uniform(const std::string &loc, const float &value1, const float &value2, const float &value3) {
	//GLint position = get_uniform_location(loc);
	GLint position = glGetUniformLocation(program_id, loc.c_str());
	glUniform3f(position, value1, value2, value3);
	CHECK_ERROR();
}

void Program::set_uniform(const std::string &loc, const glm::vec3 &value) {
	//GLint position = get_uniform_location(loc);
	GLint position = glGetUniformLocation(program_id, loc.c_str());
	glUniform3f(position, value[0], value[1], value[2]);
	CHECK_ERROR();
}

void Program::set_uniform(const std::string &loc, const glm::mat4 &value) {
	GLint position = get_uniform_location(loc);
	glUniformMatrix4fv(position, 1, false, glm::value_ptr(value));
}
