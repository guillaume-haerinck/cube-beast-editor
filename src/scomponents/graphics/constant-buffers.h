#pragma once

#include <array>
#include <string>

/**
 * @note The layouts are accessible on the graphics/ConstantBuffer.h file
 */
enum class ConstantBufferIndex {
	PER_NI_MESH = 0,
	PER_FRAME,
	PER_MATERIAL_CHANGE,
	PER_LIGHT_CHANGE,
	_CB_MAX
};

/**
 * @brief Constant buffers used by shader. This structure is used to update constant buffers.
 */
struct ConstantBuffer {
	unsigned int bufferId;
	unsigned int byteWidth = 0;
	std::string name;
};

class ConstantBuffers {
public:
	const ConstantBuffer& at(ConstantBufferIndex id) {
		// TODO
		return m_cbs.at(0);
	}

private:
	std::array<ConstantBuffer, 1> m_cbs;

private:
	friend class ViewportGui;
};
