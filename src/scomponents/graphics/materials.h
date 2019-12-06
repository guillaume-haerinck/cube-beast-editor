#pragma once

#include <vector>

/**
 * @brief CPU-side data for Materials.
 *		  Will be transformed to corresponding constant buffer structure (array of materials)
 *		  and sent over to GPU if there is a change.
 */
namespace scomp {
	/**
	 * @brief Structure of data for one Phong material
	 */
	struct Material {
		
	};

	/**
	 * @brief Unique array of Phong materials used for every objects of the scene
	 *
	 * @note Get the constant buffer corresponding to Materials from the shader component.
	 *		 It will be updated in the render system if there is a change.
	 */
	struct Materials {
		std::vector<Material> materials;
		bool hasToBeUpdated = false;
	};
}
