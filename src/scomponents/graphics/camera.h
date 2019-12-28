#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>


/**
 * @brief Arcball camera with DOF movement support. Uses polar coordinates.
 */
class Camera {
public:
	Camera() {
		proj = glm::perspectiveFovLH(glm::quarter_pi<float>(), 500.0f, 500.0f, 0.1f, 100.0f);
		view = glm::lookAtLH(position, target, glm::vec3(0, up, 0));
	}

	bool hasToBeUpdated() { return m_hasToBeUpdated; }

public:

	glm::mat4x4 view;
	glm::mat4x4 proj;
	glm::vec3 position = { 8, 5, 7 };
	glm::vec3 target = { 5, 5, 5 };

	float radius = 17;
	float up = 1;
	float phi = 1;
	float theta = 1;

private:

	bool m_hasToBeUpdated = false;

private:
	friend class CameraSystem;		
};
