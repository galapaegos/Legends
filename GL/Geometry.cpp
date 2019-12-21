#include "stdafx.h"

#include <chrono>
#include <thread>

#include "Geometry.h"

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 tex_coordinate;
};

Geometry::Geometry() {
	initialized = 0;
	bound = 0;
}

Geometry::~Geometry() {
}

void Geometry::create() {
	glGenVertexArrays(1, &vertex_array);
	glGenBuffers(1, &vertex_buffer);
	glGenBuffers(1, &index_buffer);
	CHECK_ERROR();
}

void Geometry::destroy() {
	if (index_buffer)
		glDeleteBuffers(1, &index_buffer);

	if (vertex_buffer)
		glDeleteBuffers(1, &vertex_buffer);

	if (vertex_array)
		glDeleteVertexArrays(1, &vertex_array);

	CHECK_ERROR();
}

void Geometry::set_indices(const std::vector<unsigned int> &i) { 
	indices = i; 
	initialized = 0; 
}

void Geometry::set_vertices(const std::vector<glm::vec3> &v) {
	vertices = v;
	initialized = 0;
}

void Geometry::set_normals(const std::vector<glm::vec3> &n) {
	normals = n;
	initialized = 0;
}

void Geometry::set_texture_coordinates(const std::vector<glm::vec2> &tc) {
	tex_coords = tc;
	initialized = 0;
}

void Geometry::uploadToGPU() {
	if (initialized)
		return;

	initialized = 1;

	if (vertices.size() != normals.size())
		compute_normals();

	std::vector<Vertex> gpu_vertices;

	for (int i = 0; i < vertices.size(); i++) {
		Vertex v;
		v.position = vertices[i];
		v.normal = normals[i];
		v.tex_coordinate = tex_coords[i];

		gpu_vertices.push_back(v);
	}

	if (gpu_vertices.size() == 0)
		return;

	glBindVertexArray(vertex_array);
	CHECK_ERROR();

	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, gpu_vertices.size() * sizeof(Vertex), &gpu_vertices[0], GL_STATIC_DRAW);
	CHECK_ERROR();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
	CHECK_ERROR();

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	CHECK_ERROR();

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	CHECK_ERROR();

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tex_coordinate));
	CHECK_ERROR();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	CHECK_ERROR();
}

void Geometry::bind() {
	if (!initialized) {
		uploadToGPU();
	}

	if (indices.size() > 0) {
		glBindVertexArray(vertex_array);
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
		CHECK_ERROR();
	}

	bound = 1;
}

void Geometry::unbind() {
	if (indices.size() > 0) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		CHECK_ERROR();
	}

	bound = 0;
}

void Geometry::compute_normals() {
	normals.clear();
	normals.resize(vertices.size());

	for (int i = 0; i < vertices.size(); i++) {
		normals[i] = glm::vec3(0.f);
	}

	unsigned int offset = 0;
	for (int i = 0; i < indices.size() / 3; i++) {
		glm::vec3 v1 = vertices[offset + 0];
		glm::vec3 v2 = vertices[offset + 1];
		glm::vec3 v3 = vertices[offset + 2];

		glm::vec3 u = v2 - v1;
		glm::vec3 v = v3 - v1;

		glm::vec3 n = cross(u, v);

		normals[offset + 0] = glm::normalize(normals[offset + 0] + n);
		normals[offset + 1] = glm::normalize(normals[offset + 1] + n);
		normals[offset + 2] = glm::normalize(normals[offset + 2] + n);

		offset += 3;
	}
}
