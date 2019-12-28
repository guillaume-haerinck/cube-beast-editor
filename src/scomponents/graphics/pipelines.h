#pragma once

#include <array>
#include <vector>
#include <cassert>

#include "scomponents/graphics/constant-buffers.h"

class RenderCommand; // Forward declaration to prevent circular inclusion

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

enum class PipelineIndex {
	PIP_GEOMETRY = 0,
	PIP_LIGHTING,
	PIP_GRID,
	PIP_GUI,
	PIP_DDRAW,
	_PIP_MAX
};

/**
 * @brief An OpenGL object which store a vertex shader with a pixel shader with their constant buffers
 */
struct Pipeline {
	unsigned int programIndex = 0;
	std::vector<ConstantBufferIndex> constantBufferIndices;
};

class Pipelines {
public:
	Pipelines() {};

	const Pipeline& at(PipelineIndex id) const {
		assert(id != PipelineIndex::_PIP_MAX && "Invalid pipeline index !");
		return m_pips.at(static_cast<unsigned int>(id));
	}

private:
	void init(RenderCommand& rcommand);

private:
	std::array<Pipeline, static_cast<unsigned int>(PipelineIndex::_PIP_MAX)> m_pips;

private:
	friend class App;
};
