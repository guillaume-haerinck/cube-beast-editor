#pragma once

#include <array>
#include <vector>
#include <cassert>

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
	RenderTargets() {};

	const RenderTarget& at(RenderTargetIndex id) const {
		assert(id != RenderTargetIndex::_RTT_MAX && "Invalid Render target index !");
		return m_rtts.at(static_cast<unsigned int>(id));
	}

private:
	std::array<RenderTarget, static_cast<unsigned int>(RenderTargetIndex::_RTT_MAX)> m_rtts;

private:
	friend class ViewportGui;
};
