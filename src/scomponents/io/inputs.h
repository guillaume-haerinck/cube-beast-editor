#pragma once

#include <glm/glm.hpp>
#include <array>
#include <vector>


enum class InputAction {
	CAM_ORBIT = 0,
	CAM_PAN,
	CAM_DOLLY,
	CAM_RESET,
	_ACTION_MAX
};

/**
 * @brief Store inputs state for current frame. Not undoable or re-doable.
 */
class Inputs {
public:
	Inputs() {
		m_actionState.fill(false);
	}

private:
	std::array<bool, 1> m_actionState;
	glm::vec2 m_mousePos = { 0, 0 };
	glm::vec2 m_NDCMousePos = { 0, 0 };
	glm::vec2 m_delta = { 0, 0 };
	short m_wheelDelta = 0;
};

