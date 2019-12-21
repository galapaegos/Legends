#include "stdafx.h"
#include "GL/Texture.h"

Texture::Texture() {
	texture_id = 0;

	texture_target = GL_TEXTURE_2D;
	texture_internal_format = GL_RGBA;
	texture_format = GL_RGBA;
	texture_type = GL_UNSIGNED_BYTE;
}

void Texture::create(GLenum target, GLenum internal_format, GLenum format, GLenum type, const bool &normalized) {
	glGenTextures(1, &texture_id);
	CHECK_ERROR();

	texture_target = target;
	texture_internal_format = internal_format;
	texture_format = format;
	texture_type = type;

	bind();

	if (normalized) {
		glTexParameteri(texture_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(texture_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	} else {
		glTexParameteri(texture_target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(texture_target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}

	unbind();
}

void Texture::destroy() {
	glDeleteTextures(1, &texture_id);
	CHECK_ERROR();
}

void Texture::bind() {
	glBindTexture(texture_target, texture_id);
	CHECK_ERROR();
}

void Texture::unbind() {
	glBindTexture(texture_target, 0);
	CHECK_ERROR();
}

void Texture::upload(unsigned char *ptr, const int &width, const int &height) {
	bind();

	texture_type = GL_UNSIGNED_BYTE;
	texture_format = GL_RED;

	glTexImage2D(texture_target, 0, texture_internal_format, width, height, 0, texture_format, texture_type, ptr);
	CHECK_ERROR();

	unbind();
}

void Texture::upload(float *ptr, const int &width, const int &height) {
	bind();

	texture_internal_format = GL_RGBA32F;
	texture_type = GL_FLOAT;
	texture_format = GL_RED;

	glTexImage2D(texture_target, 0, texture_internal_format, width, height, 0, texture_format, texture_type, ptr);
	CHECK_ERROR();

	unbind();
}

void Texture::upload(glm::vec4 *ptr, const int &width, const int &height) {
	bind();

	texture_internal_format = GL_RGBA32F;
	texture_type = GL_FLOAT;
	texture_format = GL_RGBA;

	glTexImage2D(texture_target, 0, texture_internal_format, width, height, 0, texture_format, texture_type, ptr);

	unbind();
}
