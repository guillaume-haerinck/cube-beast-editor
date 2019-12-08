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

struct SingletonComponents {
	std::vector<scomp::Pipeline> pipelines;
	std::array<scomp::ConstantBuffer, scomp::ConstantBufferIndex::_CONST_BUFFER_MAX> constantBuffers;
    std::array<scomp::RenderTargets, scomp::RenderTargetsType::_RTT_MAX> renderTargets;
	scomp::Mesh cubeMesh;
	scomp::Camera camera;
	scomp::Inputs inputs;
};
