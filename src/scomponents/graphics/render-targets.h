#pragma once

namespace scomp {
    enum RenderTargetsIndex {
        RTT_GEOMETRY = 0,
        _RTT_MAX
    };

	/**
	 * @brief 
	 */
	struct RenderTargets {
		unsigned int frameBufferId;
	};
}
