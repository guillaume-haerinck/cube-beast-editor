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
#include "scomponents/io/selection.h"

enum class BrushType {
	VOXEL = 0,
	FACE,
	RECTANGLE,
	LINE,
	CIRCLE
};

enum class BrushUse {
	ADD = 0,
	REMOVE,
	PAINT,
	SELECT
};

/**
 * @brief Global object used to store the state of the app. Read-only for vast-majority of systems.
 */
struct SingletonComponents {
	// Graphics
	Meshes meshes;
	Pipelines pipelines;
	ConstantBuffers constantBuffers;
	RenderTargets renderTargets;
	Camera camera;
	
	// IO
	glm::ivec2 viewportSize = { 600, 600 };
	glm::ivec2 viewportPosTopLeft = { 0, 0 };
	scomp::Inputs inputs;
	scomp::Hovered hovered;
	bool isViewportHovered = false;
	BrushType currentBrush = BrushType::VOXEL;
	BrushUse currentBushUse = BrushUse::ADD;
	bool isBrushStarted = false;
};
