#pragma once

#include <vector>
#include <array>

#include "scomponents/graphics/pipelines.h"
#include "scomponents/graphics/constant-buffers.h"
#include "scomponents/graphics/camera.h"
#include "scomponents/graphics/materials.h"
#include "scomponents/graphics/mesh.h"
#include "scomponents/graphics/render-targets.h"
#include "scomponents/io/inputs.h"
#include "scomponents/io/history.h"
#include "scomponents/io/selection.h"

/**
 * @brief Global object used to store the state of the app. Is supposed to be read-only for vast-majority of systems.
 */
struct SingletonComponents {
	// Graphics
	std::array<scomp::Pipeline, scomp::PipelineIndex::_PIP_MAX> pipelines;
	std::array<scomp::ConstantBuffer, scomp::ConstantBufferIndex::_CONST_BUFFER_MAX> constantBuffers;
    std::array<scomp::RenderTargets, scomp::RenderTargetsIndex::_RTT_MAX> renderTargets;
	scomp::Mesh cubeMesh;
	scomp::Mesh planeMesh;
	scomp::Mesh invertCubeMesh;
	scomp::VertexBuffer ddrawVb;
	scomp::Camera camera;
	
	// IO
	glm::ivec2 viewportSize = { 600, 600 };
	glm::ivec2 viewportPosTopLeft = { 0, 0 };
	scomp::Inputs inputs;
	scomp::Hovered hovered;
	bool isViewportHovered = false;
};
