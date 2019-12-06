#pragma once

#include <vector>
#include <array>

#include "scomponents/graphics/pipelines.h"
#include "scomponents/graphics/constant-buffers.h"
#include "scomponents/graphics/camera.h"
#include "scomponents/graphics/materials.h"
#include "scomponents/graphics/mesh.h"

struct SingletonComponents {
	std::vector<scomp::Pipeline> pipelines;
	std::array<scomp::ConstantBuffer, scomp::ConstantBufferIndex::_CONST_BUFFER_MAX> constantBuffers;
	scomp::Mesh cubeMesh;
};
