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

	bool isEnabled(InputAction action) const {
		assert(action != InputAction::_ACTION_MAX && "Invalid InputAction !");
		return m_actionState.at(static_cast<unsigned int>(action)); 
	}
	const glm::vec2& mousePos() const { return m_mousePos; }
	const glm::vec2& ndcMousePos() const { return m_ndcMousePos; }
	const glm::vec2& posDelta() const { return m_posDelta; }
	short wheelDelta() const { return m_wheelDelta; }

private:
	std::array<bool, static_cast<unsigned int>(InputAction::_ACTION_MAX)> m_actionState;
	glm::vec2 m_mousePos = { 0, 0 };
	glm::vec2 m_ndcMousePos = { 0, 0 };
	glm::vec2 m_posDelta = { 0, 0 };
	short m_wheelDelta = 0;

private:
	friend class App;
};

