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
		PER_FRAME = 0,
		//PER_LIGHT_CHANGE,
		//PER_COOK_MAT_CHANGE,
		//PER_PHONG_MAT_CHANGE,
		_CONST_BUFFER_MAX
	};
}
