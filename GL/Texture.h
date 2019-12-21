#ifndef _GL_Texture_h_
#define _GL_Texture_h_

#include <GL/glew.h>

#include <glm/glm.hpp>

class Texture {
public:
	Texture();

	void create(GLenum target=GL_TEXTURE_2D, GLenum internal_format=GL_RGBA, GLenum format=GL_RGBA, GLenum type=GL_UNSIGNED_BYTE, 
		const bool &normalized=true);
	void destroy();

	void bind();
	void unbind();

	void upload(unsigned char *ptr, const int &width, const int &height);
	void upload(float *ptr, const int &width, const int &height);
	void upload(glm::vec4 *ptr, const int &width, const int &height);

protected:
	GLuint texture_id;

	GLenum texture_target;
	GLenum texture_internal_format;
	GLenum texture_format;
	GLenum texture_type;
};

#endif
