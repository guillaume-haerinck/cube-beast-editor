#include "camera-system.h"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>

#include <spdlog/spdlog.h>

CameraSystem::CameraSystem(SingletonComponents& scomps) : m_scomps(scomps)
{
}

CameraSystem::~CameraSystem()
{
}

void CameraSystem::update() {
	// ArcBall rotation
	if (m_scomps.inputs.actionState.at(scomp::InputAction::CAM_ORBIT)) {
		m_scomps.camera.m_theta -= m_scomps.inputs.delta.x * 0.01f;
		m_scomps.camera.m_phi += m_scomps.inputs.delta.y * 0.01f;

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

	// Move target and camera position from their local coordinate system
	// https://stackoverflow.com/questions/46978948/move-a-vertex-along-a-plane-given-the-plane-normal
	// https://stackoverflow.com/questions/35285289/translate-3d-points-along-arbitrary-plane
	// https://www.youtube.com/watch?v=-Fn4atv2NsQ
	// https://www.khanacademy.org/math/linear-algebra/alternate-bases/change-of-basis/v/lin-alg-changing-coordinate-systems-to-help-find-a-transformation-matrix
	if (m_scomps.inputs.actionState.at(scomp::InputAction::CAM_PAN)) {
	 	glm::vec4 col0 = glm::normalize(glm::column<glm::mat4x4>(m_scomps.camera.m_view, 0));
		glm::vec4 col1 = glm::normalize(glm::column<glm::mat4x4>(m_scomps.camera.m_view, 1));


		// TODO

		// m_scomps.camera.hasToBeUpdated = true;
	}

	// Change arcball radius
	if (m_scomps.inputs.actionState.at(scomp::InputAction::CAM_DOLLY)) {
		if (m_scomps.inputs.wheelDelta > 0 && m_scomps.camera.m_radius >= 2) {
			m_scomps.camera.m_radius -= 1;
		} else if (m_scomps.inputs.wheelDelta < 0) {
			m_scomps.camera.m_radius += 1;
		}

		m_scomps.camera.m_hasToBeUpdated = true;
	}

	// Reset zoom and position
	if (m_scomps.inputs.actionState.at(scomp::InputAction::CAM_RESET)) {
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
		m_scomps.camera.m_hasToBeUpdated = false;
	}
}
