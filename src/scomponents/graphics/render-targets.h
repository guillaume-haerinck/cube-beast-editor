#pragma once

namespace scomp {
    enum RenderTargetsType {
        RTT_PICKING = 0,
        _RTT_MAX
    };

	/**
	 * @brief 
	 */
	struct RenderTargets {
		unsigned int frameBufferId;
	};
}
