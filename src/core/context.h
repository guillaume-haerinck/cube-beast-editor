#pragma once

#include <met/met.hpp>
#include "graphics/render-command.h"
#include "singleton-components.h"

struct Context {
	Context(SingletonComponents& scomps) : rcommand(scomps) {}

	met::Registry registry;
	RenderCommand rcommand;
};
