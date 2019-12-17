#include "camera-system.h"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>

CameraSystem::CameraSystem(SingletonComponents& scomps) : m_scomps(scomps)
{
}

CameraSystem::~CameraSystem()
{
}

void CameraSystem::update() {
	// ArcBall rotation
	if (m_scomps.inputs.actionState.at(scomp::InputAction::CAM_ORBIT)) {
		m_scomps.camera.theta -= m_scomps.inputs.delta.x * 0.01f;
		m_scomps.camera.phi += m_scomps.inputs.delta.y * 0.01f;

		// Keep phi within -2PI to +2PI for easy 'up' comparison
		if (m_scomps.camera.phi > glm::two_pi<float>())   {
			m_scomps.camera.phi -= glm::two_pi<float>();
		} else if (m_scomps.camera.phi < -glm::two_pi<float>()) {
			m_scomps.camera.phi += glm::two_pi<float>();
		}

		// If phi is between 0 to PI or -PI to -2PI, make 'up' be positive Y, other wise make it negative Y
		if ((m_scomps.camera.phi > 0 && m_scomps.camera.phi < glm::pi<float>()) || (m_scomps.camera.phi < -glm::pi<float>() && m_scomps.camera.phi > -glm::two_pi<float>())) {
			m_scomps.camera.up = 1.0f;
		} else {
			m_scomps.camera.up = -1.0f;
		}

		m_scomps.camera.hasToBeUpdated = true;
	}

	// Move along vertical plane
	if (m_scomps.inputs.actionState.at(scomp::InputAction::CAM_PAN)) {
		const glm::mat3 invView = glm::inverse(m_scomps.camera.view);
		glm::vec3 dx = invView * glm::vec3(1.0f, 0.0f, 0.0f);
		glm::vec3 dy = invView * glm::vec3(0.0f, 1.0f, 0.0f);
		m_scomps.camera.target += (dy * m_scomps.inputs.delta.y - dx * m_scomps.inputs.delta.x);

		m_scomps.camera.hasToBeUpdated = true;
	}

	// Change arcball radius
	if (m_scomps.inputs.actionState.at(scomp::InputAction::CAM_DOLLY)) {
		if (m_scomps.inputs.wheelDelta > 0 && m_scomps.camera.radius >= 2) {
			m_scomps.camera.radius -= 1;
		} else if (m_scomps.inputs.wheelDelta < 0) {
			m_scomps.camera.radius += 1;
		}

		m_scomps.camera.hasToBeUpdated = true;
	}

	// Reset zoom and position
	if (m_scomps.inputs.actionState.at(scomp::InputAction::CAM_RESET)) {
		// TODO
	}

	// Update camera position
	if (m_scomps.camera.hasToBeUpdated == true) {
		m_scomps.camera.position.x = m_scomps.camera.radius * sinf(m_scomps.camera.phi) * sinf(m_scomps.camera.theta);
		m_scomps.camera.position.y = m_scomps.camera.radius * cosf(m_scomps.camera.phi);
		m_scomps.camera.position.z = m_scomps.camera.radius * sinf(m_scomps.camera.phi) * cosf(m_scomps.camera.theta);

        glm::vec3 eye = m_scomps.camera.position;
        glm::vec3 target = m_scomps.camera.target;
        glm::vec3 up = glm::vec3(0, m_scomps.camera.up, 0);

        m_scomps.camera.view = glm::lookAtLH(eye, target, up);
		m_scomps.camera.hasToBeUpdated = false;
	}
}
