#pragma once

#include <array>
#include <cassert>

#include "scomponents/io/viewport.h"

class RenderCommand; // Forward declaration to prevent circular inclusion

enum class TextureIndex {
	UI = 0,
	_TEX_MAX
};

/**
 * @brief 
 */
struct Texture {
	unsigned int id;
	unsigned int slot = 0;
};

class Textures {
public:
	Textures() {};

	const Texture& at(TextureIndex id) const {
		assert(id != TextureIndex::_TEX_MAX && "Invalid texture index !");
		return m_txts.at(static_cast<unsigned int>(id));
	}

private:
	void init(RenderCommand& rcommand);
	void destroy(RenderCommand& rcommand);

private:
	std::array<Texture, static_cast<unsigned int>(TextureIndex::_TEX_MAX)> m_txts;

private:
	friend class App;
};
