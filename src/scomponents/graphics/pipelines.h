#pragma once

#include <array>
#include <vector>

#include "scomponents/graphics/constant-buffers.h"

namespace scomp {
	/**
	 * @brief A vertex shader. The first stage in the graphic pipeline.
	 */
	struct VertexShader {
		unsigned int shaderId;
	};

	/**
	 * @brief A pixel shader. The last stage in the graphic pipeline.
	 */
	struct FragmentShader {
		unsigned int shaderId;
	};

	/**
	 * @brief An OpenGL object whicch store a vertex shader with a pixel shader with their constant buffers
	 */
	struct Pipeline {
		unsigned int programIndex = 0;
		std::vector<scomp::ConstantBufferIndex> constantBufferIndices;
	};
};
