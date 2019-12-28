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
		m_proj = glm::perspectiveFovLH(glm::quarter_pi<float>(), 500.0f, 500.0f, 0.1f, 100.0f);
		m_view = glm::lookAtLH(m_position, m_target, glm::vec3(0, m_up, 0));
	}

	const bool hasToBeUpdated() const { return m_hasToBeUpdated; }
	const glm::mat4x4& view() const { return m_view; }
	const glm::mat4x4& proj() const { return m_proj; }
	const glm::vec3& position() const { return m_position; }

private:
	glm::mat4x4 m_view;
	glm::mat4x4 m_proj;
	glm::vec3 m_position = { 8, 5, 7 };
	glm::vec3 m_target = { 5, 5, 5 };

	float m_radius = 17;
	float m_up = 1;
	float m_phi = 1;
	float m_theta = 1;
	bool m_hasToBeUpdated = false;

private:
	friend class CameraSystem;
	friend class ViewportGui; // temp
};
