#pragma once

#include "scomponents/graphics/materials.h"

namespace comp {
	/**
	 * @brief Material used by cubes
	 * 
	 * @param materialIndex - The index in the std::vector of Materials singleton component (0 if default material).
	 *						  The same material can be shared by multiple mesh.
	 */
	struct Material {
		unsigned int materialIndex = 0;
		scomp::MaterialType materialType = scomp::MaterialType::NO_MATERIAL;
	};
}
