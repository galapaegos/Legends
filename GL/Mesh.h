#ifndef _GL_Mesh_h_
#define _GL_Mesh_h_

#include "stdafx.h"

#include "GL/Camera.h"
#include "GL/Geometry.h"
#include "GL/Program.h"

class Mesh {
public:
	Mesh(std::shared_ptr<Geometry> geom, std::shared_ptr<Program> prog);

	void create();
	void destroy();

	void render(std::shared_ptr<Camera> camera);

protected:
	std::shared_ptr<Geometry> geometry;
	std::shared_ptr<Program> material;

	int visible;
};

#endif
