#pragma once

#include "graphics/render-command.h"
#include "graphics/debug-draw.h"
#include "data/singleton-components.h"
#include "history/history-handler.h"

/**
 * @brief Global object used accross systems
 * TODO Remove and store data directly in App class. Refractoring WIP to reduce global coupling.
 * Can use service locator pattern or singleton for history and debug draw
 */
struct Context {
	Context(SingletonComponents& scomps) : ddraw(rcommand, scomps), history(scomps) {}

	RenderCommand rcommand;
	DebugDraw ddraw;
	HistoryHandler history;
};
