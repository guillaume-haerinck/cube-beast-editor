#include "camera-system.h"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <profiling/instrumentor.h>

#include <spdlog/spdlog.h>

CameraSystem::CameraSystem() {}

CameraSystem::~CameraSystem() {}

void CameraSystem::update(Camera& cam, const Inputs& inputs) {
	PROFILE_SCOPE("CameraSystem update");

	// ArcBall rotation
	if (inputs.isEnabled(InputAction::CAM_ORBIT)) {
		cam.m_theta -= inputs.posDelta().x * 0.01f;
		cam.m_phi += inputs.posDelta().y * 0.01f;

		// Keep phi within -2PI to +2PI for easy 'up' comparison
		if (cam.m_phi > glm::two_pi<float>())   {
			cam.m_phi -= glm::two_pi<float>();
		} else if (cam.m_phi < -glm::two_pi<float>()) {
			cam.m_phi += glm::two_pi<float>();
		}

		// If phi is between 0 to PI or -PI to -2PI, make 'up' be positive Y, other wise make it negative Y
		if ((cam.m_phi > 0 && cam.m_phi < glm::pi<float>()) || (cam.m_phi < -glm::pi<float>() && cam.m_phi > -glm::two_pi<float>())) {
			cam.m_up = 1.0f;
		} else {
			cam.m_up = -1.0f;
		}

		cam.m_hasToBeUpdated = true;
	}

	// Move target from cam coordinate system
	if (inputs.isEnabled(InputAction::CAM_PAN)) {
		glm::mat4x4 invView = glm::inverse(cam.m_view);
	 	glm::vec4 col0 = glm::normalize(glm::column<glm::mat4x4>(invView, 0));
		glm::vec4 col1 = glm::normalize(glm::column<glm::mat4x4>(invView, 1));
		glm::vec4 movement = col0 * inputs.posDelta().x + col1 * -inputs.posDelta().y;

		cam.m_target += glm::vec3(movement.x, movement.y, movement.z) * 0.04f;

		cam.m_hasToBeUpdated = true;
	}

	// Change arcball radius
	if (inputs.isEnabled(InputAction::CAM_DOLLY)) {
		if (inputs.wheelDelta() > 0 && cam.m_radius >= 2) {
			cam.m_radius -= 1.5f;
		} else if (inputs.wheelDelta() < 0) {
			cam.m_radius += 1.5f;
		}

		cam.m_hasToBeUpdated = true;
	}

	// Reset zoom and position
	if (inputs.isEnabled(InputAction::CAM_RESET)) {
		// TODO
	}

	// Update cam position
	if (cam.hasToBeUpdated() == true) {
		cam.m_position.x = cam.m_target.x + cam.m_radius * sinf(cam.m_phi) * sinf(cam.m_theta);
		cam.m_position.y = cam.m_target.y + cam.m_radius * cosf(cam.m_phi);
		cam.m_position.z = cam.m_target.z + cam.m_radius * sinf(cam.m_phi) * cosf(cam.m_theta);

        glm::vec3 eye = cam.m_position;
        glm::vec3 target = cam.m_target;
        glm::vec3 up = glm::vec3(0, cam.m_up, 0);

        cam.m_view = glm::lookAtLH(eye, target, up);
	}
}
