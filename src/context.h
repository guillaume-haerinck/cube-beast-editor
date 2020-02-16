#pragma once

#include "graphics/render-command.h"
#include "graphics/debug-draw.h"
#include "scomponents/singleton-components.h"
#include "history/history-handler.h"

/**
 * @brief Global object used accross systems
 */
struct Context {
	Context(SingletonComponents& scomps) : ddraw(rcommand, scomps), history(scomps) {}

	RenderCommand rcommand;
	DebugDraw ddraw;
	HistoryHandler history;
};
