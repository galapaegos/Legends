// stdafx.cpp : source file that includes just the standard includes
// Legends.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file

void check_opengl_error(const char *file, const int &line) {
	GLenum error = glGetError();

	bool found_error = false;
	std::vector<std::string> errors;
	while (error != GL_NO_ERROR) {
		std::string e = format("%s", gluErrorString(error));
		printf("OpenGL Error: [%s:%i] %s\n", __FILE__, __LINE__, e.c_str());

		found_error = true;
		errors.push_back(e);
		error = glGetError();
	}
	
	if (found_error)
		DebugBreak(); 
}