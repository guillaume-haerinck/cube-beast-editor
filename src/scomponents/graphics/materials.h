#pragma once

#include <vector>

/**
 * @brief CPU-side data for Materials.
 *		  Will be transformed to corresponding constant buffer structure (array of materials)
 *		  and sent over to GPU if there is a change.
 */
namespace scomp {

	enum MaterialType {
		NO_MATERIAL,
		PHONG,
		COOK_TORRANCE
	};

	///////////////////////////////////////////////////////////////////////////
	//////////////////////////////// BLINN-PHONG //////////////////////////////
	///////////////////////////////////////////////////////////////////////////

	/**
	 * @brief Structure of data for one Phong material
	 */
	struct PhongMaterial {
		
	};

	/**
	 * @brief Unique array of Phong materials used for every objects of the scene
	 *
	 * @note Get the constant buffer corresponding to Materials from the shader component.
	 *		 It will be updated in the render system if there is a change.
	 */
	struct PhongMaterials {
		std::vector<PhongMaterial> materials;
		bool hasToBeUpdated = false;
	};
}
