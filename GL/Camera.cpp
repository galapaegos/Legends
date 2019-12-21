#include "stdafx.h"

#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Camera::Camera(const float &v, const float &a, const float &n, const float &f) {
	fov = v;
	aspect = a;
	near_plane = n;
	far_plane = f;

	projection = glm::perspective(fov, aspect, near_plane, far_plane);
	CHECK_ERROR();

	glGenBuffers(1, &camera_ubo);
	glBindBuffer(GL_UNIFORM_BUFFER, camera_ubo);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 10 + sizeof(glm::vec4) * 1, nullptr, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	CHECK_ERROR();
}

void Camera::setPosition(const glm::vec3 &p, const glm::vec3 &l) {
	position = glm::vec4(p, 1);
	viewing = l;

	view = glm::lookAt(p, l, glm::vec3(0, 1, 0));
}

void Camera::bind() {
	glViewport(0, 0, screen_size[0], screen_size[1]);
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, camera_ubo, 0, sizeof(glm::mat4) * 10 + sizeof(glm::vec4) * 1);
	CHECK_ERROR();

	glBindBuffer(GL_UNIFORM_BUFFER, camera_ubo);
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 0, sizeof(glm::mat4), glm::value_ptr(projection));
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 1, sizeof(glm::mat4), glm::value_ptr(view));

	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 10, sizeof(glm::vec4), glm::value_ptr(position));
	CHECK_ERROR();
}

void Camera::unbind() {
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	CHECK_ERROR();
}

void Camera::resize(const int &width, const int &height) {
	screen_size[0] = width;
	screen_size[1] = height;

	aspect = float(width) / float(height);

	projection = glm::perspective(fov, aspect, near_plane, far_plane);
}

glm::vec3 Camera::screenToWorld(const glm::vec2 &screen) {
	return glm::project(glm::vec3(screen, 1), view, projection, glm::vec4(0, 0, screen_size[0], screen_size[1]));
}

glm::vec3 Camera::worldToScreen(const glm::vec3 &world) {
	return glm::unProject(world, view, projection, glm::vec4(0, 0, screen_size[0], screen_size[1]));
}
