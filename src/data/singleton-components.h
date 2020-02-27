#pragma once

#include "data/graphics/pipelines.h"
#include "data/graphics/constant-buffers.h"
#include "data/graphics/camera.h"
#include "data/graphics/materials.h"
#include "data/graphics/meshes.h"
#include "data/graphics/render-targets.h"
#include "data/graphics/textures.h"
#include "data/graphics/lights.h"

#include "data/io/inputs.h"
#include "data/io/hovered.h"
#include "data/io/viewport.h"
#include "data/io/brush.h"
#include "data/graphics/ui-style.h"

/**
 * @brief Global object used to store the state of the app. 
 * @note Only store data, it has no logic. Read-only for vast-majority of systems.
 * TODO being refractored to reduce global coupling. Data saved here but not acced through this struct
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

	// Voxels
	

};
