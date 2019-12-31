#pragma once

#include <glm/glm.hpp>

/**
 * @brief Structure for constant buffers used by shaders and updated on CPU side in render system
 * @note Sorted and named by their frequency of updates. Their bytesize is a multiple of 16.
 *		 char = 1 / float = 4 / uint32_t = 4 / glm::vec3 = 12 / glm::mat4 = 64
 *		 16, 32, 48, 64, 80, 96, 112, 128, 144, 160, 176, 192, 208, 224, 240, 256, ...
 *
 * @link http://vzout.com/directx12/const-buffer-performance.html
 */
namespace cb {
	/**
	 * @brief Constant buffer updated once per non instanced mesh
	 */
	struct perNiMesh {
		glm::mat4x4 matWorld;
	};

	/**
	 * @brief Constant buffer updated once per frame
	 */
	struct perFrame {
		glm::mat4x4 matViewProj;
		glm::vec3 cameraPos;
		float debug;
	};

	/**
	 * @brief Constant buffer updated when a PBR material is changed
	 * @note Is sent as an array.
	 */
	struct perMaterialChange {
		glm::vec3 albedo;
		float emissiveFactor;
	};

	/**
	 * @brief Constant buffer updated when a light is changed
	 */
	namespace perLightChange {
		struct perShadowPass {
			glm::mat4x4 matViewProj_lightSpace;
		};

		struct DirectionalLight {
			glm::vec3 direction;
			float intensity;
			glm::vec3 color;
			char padding[4];
		};

		struct PointLight {
			glm::vec3 position;
			float intensity;
			glm::vec3 color;
			float attenuationRadius;
		};

		struct SpotLight {
			glm::vec3 direction;
			float intensity;
			glm::vec3 position;
			float spotAngle;
			glm::vec3 color;
			float attenuationRadius;
		};
	}
}
