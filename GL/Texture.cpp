#include "stdafx.h"
#include "GL/Texture.h"

Texture::Texture(){}

void Texture::create() {}
void Texture::destroy() {}

void Texture::bind() {
	//glBindTexture(GL_TEXTURE_2D,)
}

void Texture::unbind() {
	glBindTexture(GL_TEXTURE_2D, 0);
}
