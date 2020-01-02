#pragma once

#include <array>
#include <vector>
#include <cassert>

#include "scomponents/io/viewport.h"

class RenderCommand; // Forward declaration to prevent circular inclusion

struct PixelBuffer {
	unsigned int bufferId;
	unsigned int readBufferSlot;
};

enum class RenderTargetIndex {
	RTT_GEOMETRY = 0,
	RTT_SHADOW_MAP,
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
	PixelBuffer pixelBuffer;
};

/**
 * @brief 
 */
class RenderTargets {
public:
	RenderTargets() {};

	const RenderTarget& at(RenderTargetIndex id) const {
		assert(id != RenderTargetIndex::_RTT_MAX && "Invalid Render target index !");
		return m_rts.at(static_cast<unsigned int>(id));
	}

private:
	void init(RenderCommand& rcommand, const Viewport& viewport);
	void destroy(RenderCommand& rcommand);

private:
	std::array<RenderTarget, static_cast<unsigned int>(RenderTargetIndex::_RTT_MAX)> m_rts;

private:
	friend class App;
	friend class ViewportGui; // temp
};
