#pragma once

// #include <met/met.hpp>
#include "graphics/render-command.h"
#include "scomponents/singleton-components.h"

/**
 * @brief Global object used accross systems
 */
struct Context {
	Context(SingletonComponents& scomps) : rcommand(scomps) {}

	// met::Registry registry;
	RenderCommand rcommand;
};
