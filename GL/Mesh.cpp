#include "stdafx.h"

#include "GL/Mesh.h"

#include <glm/gtc/type_ptr.hpp>

Mesh::Mesh(std::shared_ptr<Geometry> geom, std::shared_ptr<Program> prog) {
	geometry = geom;
	material = prog;

	visible = 1;
}

void Mesh::create() { 
	geometry->create();
	material->create();
}

void Mesh::destroy() {
	geometry->destroy();
	material->destroy();
}

void Mesh::render(std::shared_ptr<Camera> camera) {
	if (!visible)
		return;

	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 4, sizeof(glm::mat4), glm::value_ptr(glm::mat4(1.f)));
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 5, sizeof(glm::mat4), glm::value_ptr(glm::inverse(glm::mat4(1.f))));
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 6, sizeof(glm::mat4), glm::value_ptr(
		camera->get_projection() * camera->get_view() * glm::mat4(1.f)));
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 7, sizeof(glm::mat4), glm::value_ptr(
		camera->get_view() * glm::mat4(1.f)));
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 8, sizeof(glm::mat4), glm::value_ptr(
		glm::transpose(camera->get_view() * glm::mat4(1.f))));
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 9, sizeof(glm::mat4), glm::value_ptr(
		camera->get_projection() * camera->get_view() * glm::mat4(1.f)));
	CHECK_ERROR();

	if (geometry && material) {
		material->bind();
		geometry->bind();

		auto indices = geometry->get_indices();
		if (indices.size() > 0) {
			glDrawElements(GL_TRIANGLES, GLsizei(indices.size()), GL_UNSIGNED_INT, 0);
			CHECK_ERROR();
		}

		geometry->unbind();
		material->unbind();
	}
}