#pragma once

#include <glm/glm.hpp>

namespace comp {
	/**
	 * @brief Position and rotation in the 3d grid
	 */
	struct Transform {
		Transform(glm::ivec3 pos = glm::ivec3(0)) : position(pos) {}

		glm::ivec3 position;
	};
}
