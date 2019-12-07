#pragma once

#include <met/met.hpp>
#include "inputs.h"
#include "components/physics/position.h"

namespace scomp {
	/**
	 * @brief Saves action history for undo and redo support
	 * 
	 * @note The action gives meaning to the parameters
	 * @example DELETE_CUBE will use 'from', but not 'to' and 'option'.
	 * @example PAINT_CUBES will use all parameters, and 'option' as color.
	 */
	struct History {
		InputAction action;
		comp::Position from;
		comp::Position to;
		glm::vec3 option;
	};
}
