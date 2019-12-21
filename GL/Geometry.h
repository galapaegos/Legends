#ifndef _GL_Geometry_h_
#define _GL_Geometry_h_

#include "stdafx.h"

#include <glm/glm.hpp>

class Geometry {
public:
	Geometry();
	~Geometry();

	void uploadToGPU();

	void create();
	void destroy();

	void bind();
	void unbind();

	void set_indices(const std::vector<unsigned int> &i);
	void set_vertices(const std::vector<glm::vec3> &v);
	void set_normals(const std::vector<glm::vec3> &n);
	void set_texture_coordinates(const std::vector<glm::vec2> &tc);

	std::vector<unsigned int> get_indices() { return indices; }
	std::vector<glm::vec3> get_vertices() { return vertices; }
	std::vector<glm::vec3> get_normals() { return normals; }
	std::vector<glm::vec2> get_texture_coordinates() { return tex_coords; }

protected:
	int initialized;
	int bound;

	void compute_normals();

	GLuint vertex_array;
	GLuint vertex_buffer;
	GLuint index_buffer;

	std::vector<unsigned int> indices;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> tex_coords;
};

#endif
