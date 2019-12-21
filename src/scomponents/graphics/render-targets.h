#pragma once

namespace scomp {
    enum RenderTargetsIndex {
        RTT_GEOMETRY = 0,
        RTT_FINAL,
        _RTT_MAX
    };

	/**
	 * @brief 
	 */
	struct RenderTargets {
		unsigned int frameBufferId;
		std::vector<unsigned int> textureIds;
		std::vector<unsigned int> renderBufferIds;
	};
}
