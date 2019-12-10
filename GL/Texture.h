#ifndef _GL_Texture_h_
#define _GL_Texture_h_

#include <GL/glew.h>

class Texture {
public:
	Texture();

	void create();
	void destroy();

	void bind();
	void unbind();

	template<class Pixel>
	void upload(Pixel *ptr, const int &width, const int &height) {
		bind();

		//glTexture2D(GL_TEXTURE_2D, );

		unbind();
	}
};

#endif
