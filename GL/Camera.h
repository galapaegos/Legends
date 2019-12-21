#ifndef _GL_Camera_h_
#define _GL_Camera_h_

#include "stdafx.h"

#include <glm/glm.hpp>

// Camera Unified Buffer Object details:
// mat4 P_Matrix // Projection Matrix
// mat4 V_Matrix // View Matrix
// mat4 I_V_Matrix // Inverse View Matrix
// mat4 VP_Matrix // View Projection Matrix
// mat4 M_Matrix // Model Matrix
// mat4 I_M_Matrix // Inverse Model Matrix
// mat4 MVP_Matrix // Model View Projection Matrix
// mat4 T_MV_Matrix // Transpose of the Model View
// mat4 N_Matrix  // Inverse Transpose of the Model View
// vec4 Camera_Position //camera position in world space

class Camera {
public:
	Camera(const float &fov, const float &aspect, const float &near, const float &far);

	void setPosition(const glm::vec3 &position, const glm::vec3 &lookat);

	void bind();
	void unbind();

	void resize(const int &width, const int &height);

	glm::vec3 screenToWorld(const glm::vec2 &screen);
	glm::vec3 worldToScreen(const glm::vec3 &world);

	glm::mat4 get_projection() const { return projection; }
	glm::mat4 get_view() const { return view; }

protected:
	GLuint camera_ubo;

	float fov;
	float aspect;
	float near_plane;
	float far_plane;

	glm::vec4 position;
	glm::vec3 viewing;

	glm::vec<2, int> screen_size;

	glm::mat4 projection;
	glm::mat4 view;
};

#endif
