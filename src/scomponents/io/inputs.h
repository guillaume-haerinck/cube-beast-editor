#pragma once

#include <glm/glm.hpp>
#include <array>
#include <vector>

namespace scomp {
	enum InputAction {
		CAM_ORBIT = 0,
		CAM_PAN,
		CAM_DOLLY,
		CAM_RESET,
		_ACTION_MAX
	};

    /**
	 * @brief Store inputs state for current frame. Not undoable or re-doable.
	 */
	struct Inputs {
		Inputs() {
			actionState.fill(false);
		}

		std::array<bool, InputAction::_ACTION_MAX> actionState;
		glm::vec2 mousePos = { 0, 0 };
		glm::vec2 NDCMousePos = { 0, 0 };
		glm::vec2 delta = { 0, 0 };
		short wheelDelta = 0;
	};
}
