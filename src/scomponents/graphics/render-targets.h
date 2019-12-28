#pragma once

#include <array>
#include <vector>

enum class RenderTargetIndex {
	RTT_GEOMETRY = 0,
	RTT_FINAL,
	_RTT_MAX
};

/**
 * @brief 
 */
struct RenderTarget {
	unsigned int frameBufferId;
	std::vector<unsigned int> textureIds;
	std::vector<unsigned int> renderBufferIds;
};

/**
 * @brief 
 */
class RenderTargets {
public:
	const RenderTarget& at(RenderTargetIndex id) {
		// TODO
		return m_rtts.at(0);
	}

private:
	std::array<RenderTarget, 1> m_rtts;

private:
	friend class ViewportGui;
};
