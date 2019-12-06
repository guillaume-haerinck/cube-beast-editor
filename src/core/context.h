#pragma once

#include <met/met.hpp>
#include <memory>
#include <vector>
#include <array>

#include "graphics/render-command.h"
#include "scomponents/graphics/pipelines.h"
#include "scomponents/graphics/constant-buffers.h"
#include "scomponents/graphics/camera.h"
#include "scomponents/graphics/materials.h"

struct Context {
	met::Registry registry;
	std::unique_ptr<RenderCommand> rcommand;
	std::vector<scomp::Pipeline> pipelines;
	std::array<scomp::ConstantBuffer, scomp::ConstantBufferIndex::_CONST_BUFFER_MAX> constantBuffers;
};
