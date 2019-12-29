#pragma once

#include "scomponents/graphics/pipelines.h"
#include "scomponents/graphics/constant-buffers.h"
#include "scomponents/graphics/camera.h"
#include "scomponents/graphics/materials.h"
#include "scomponents/graphics/meshes.h"
#include "scomponents/graphics/render-targets.h"
#include "scomponents/graphics/textures.h"
#include "scomponents/graphics/lights.h"

#include "scomponents/io/inputs.h"
#include "scomponents/io/hovered.h"
#include "scomponents/io/viewport.h"
#include "scomponents/io/brush.h"
#include "scomponents/graphics/ui-style.h"

/**
 * @brief Global object used to store the state of the app. 
 * @note Only store data, it has no logic. Read-only for vast-majority of systems.
 */
struct SingletonComponents {
	SingletonComponents() {};

	// Graphics
	Meshes meshes;
	Pipelines pipelines;
	ConstantBuffers constantBuffers;
	RenderTargets renderTargets;
	Textures textures;
	Materials materials;
	Lights lights;
	Camera camera;
	UIStyle uiStyle;
	
	// IO
	Inputs inputs;
	Hovered hovered;
	Viewport viewport;
	Brush brush;
};
