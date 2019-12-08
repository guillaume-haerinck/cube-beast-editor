#pragma once

namespace scomp {
    enum RenderTargetsIndex {
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
