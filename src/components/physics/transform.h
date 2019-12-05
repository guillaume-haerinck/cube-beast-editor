#pragma once

#include <glm/glm.hpp>

namespace comp {
	/**
	 * @brief Store position, rotation and scaling of an entity in the world
	 */
    // TODO use integrer for position ?
	struct Transform {
		glm::vec3 position;
		float scale;
	};
}
