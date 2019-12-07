#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>

namespace scomp {
	/**
	 * @brief Arcball camera with DOF movement support. Uses polar coordinates.
	 */
	struct Camera {
		glm::mat4x4 view;
		glm::mat4x4 proj;
		glm::vec3 position = { 0, 0, 6 };
		glm::vec3 target = { 0, 0, 0 };

		float radius = 5;
		float up = 1;
		float phi = 0;
		float theta = 0;

		bool hasToBeUpdated = false;

		Camera() {
			proj = glm::perspectiveFovLH(glm::quarter_pi<float>(), 500.0f, 500.0f, 0.1f, 100.0f);
			view = glm::translate(glm::mat4x4(1.0f), glm::vec3(0.0f, 0.0f, 6.0f));
		}
	};
}
