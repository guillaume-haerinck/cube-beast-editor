#pragma once

#include <array>
#include <string>
#include <cassert>

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
	ConstantBuffers() {};

	const ConstantBuffer& at(ConstantBufferIndex id) const {
		assert(id != ConstantBufferIndex::_CB_MAX);
		return m_cbs.at(static_cast<unsigned int>(id));
	}

private:
	std::array<ConstantBuffer, static_cast<unsigned int>(ConstantBufferIndex::_CB_MAX)> m_cbs;

private:
	friend class ViewportGui;
};
