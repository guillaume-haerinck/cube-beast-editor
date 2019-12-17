#pragma once

#include <array>
#include <string>

namespace scomp {
	/**
	 * @brief Constant buffers used by shader. This structure is used to update constant buffers.
	 */
	struct ConstantBuffer {
		unsigned int bufferId;
		unsigned int byteWidth = 0;
		std::string name;
	};

	/**
	 * @note The layouts are accessible on the graphics/ConstantBuffer.h file
	 */
	enum ConstantBufferIndex {
		PER_NI_MESH = 0,
		PER_FRAME,
		PER_MATERIAL_CHANGE,
		PER_LIGHT_CHANGE,
		_CONST_BUFFER_MAX
	};
}
