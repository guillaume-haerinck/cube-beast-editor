#include "camera-system.h"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <profiling/instrumentor.h>

#include <spdlog/spdlog.h>

CameraSystem::CameraSystem(SingletonComponents& scomps) : m_scomps(scomps)
{
}

CameraSystem::~CameraSystem()
{
}

void CameraSystem::update() {
	PROFILE_SCOPE("CameraSystem update");

	// ArcBall rotation
	if (m_scomps.inputs.isEnabled(InputAction::CAM_ORBIT)) {
		m_scomps.camera.m_theta -= m_scomps.inputs.posDelta().x * 0.01f;
		m_scomps.camera.m_phi += m_scomps.inputs.posDelta().y * 0.01f;

		// Keep phi within -2PI to +2PI for easy 'up' comparison
		if (m_scomps.camera.m_phi > glm::two_pi<float>())   {
			m_scomps.camera.m_phi -= glm::two_pi<float>();
		} else if (m_scomps.camera.m_phi < -glm::two_pi<float>()) {
			m_scomps.camera.m_phi += glm::two_pi<float>();
		}

		// If phi is between 0 to PI or -PI to -2PI, make 'up' be positive Y, other wise make it negative Y
		if ((m_scomps.camera.m_phi > 0 && m_scomps.camera.m_phi < glm::pi<float>()) || (m_scomps.camera.m_phi < -glm::pi<float>() && m_scomps.camera.m_phi > -glm::two_pi<float>())) {
			m_scomps.camera.m_up = 1.0f;
		} else {
			m_scomps.camera.m_up = -1.0f;
		}

		m_scomps.camera.m_hasToBeUpdated = true;
	}

	// Move target from camera coordinate system
	if (m_scomps.inputs.isEnabled(InputAction::CAM_PAN)) {
		glm::mat4x4 invView = glm::inverse(m_scomps.camera.m_view);
	 	glm::vec4 col0 = glm::normalize(glm::column<glm::mat4x4>(invView, 0));
		glm::vec4 col1 = glm::normalize(glm::column<glm::mat4x4>(invView, 1));
		glm::vec4 movement = col0 * m_scomps.inputs.posDelta().x + col1 * -m_scomps.inputs.posDelta().y;

		m_scomps.camera.m_target += glm::vec3(movement.x, movement.y, movement.z) * 0.02f;

		m_scomps.camera.m_hasToBeUpdated = true;
	}

	// Change arcball radius
	if (m_scomps.inputs.isEnabled(InputAction::CAM_DOLLY)) {
		if (m_scomps.inputs.wheelDelta() > 0 && m_scomps.camera.m_radius >= 2) {
			m_scomps.camera.m_radius -= 1;
		} else if (m_scomps.inputs.wheelDelta() < 0) {
			m_scomps.camera.m_radius += 1;
		}

		m_scomps.camera.m_hasToBeUpdated = true;
	}

	// Reset zoom and position
	if (m_scomps.inputs.isEnabled(InputAction::CAM_RESET)) {
		// TODO
	}

	// Update camera position
	if (m_scomps.camera.hasToBeUpdated() == true) {
		m_scomps.camera.m_position.x = m_scomps.camera.m_target.x + m_scomps.camera.m_radius * sinf(m_scomps.camera.m_phi) * sinf(m_scomps.camera.m_theta);
		m_scomps.camera.m_position.y = m_scomps.camera.m_target.y + m_scomps.camera.m_radius * cosf(m_scomps.camera.m_phi);
		m_scomps.camera.m_position.z = m_scomps.camera.m_target.z + m_scomps.camera.m_radius * sinf(m_scomps.camera.m_phi) * cosf(m_scomps.camera.m_theta);

        glm::vec3 eye = m_scomps.camera.m_position;
        glm::vec3 target = m_scomps.camera.m_target;
        glm::vec3 up = glm::vec3(0, m_scomps.camera.m_up, 0);

        m_scomps.camera.m_view = glm::lookAtLH(eye, target, up);
	}
}
