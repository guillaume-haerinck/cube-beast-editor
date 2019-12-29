#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <cassert>

#include "graphics/constant-buffer.h"

class Materials {
public:
	Materials() {
		loadDefaultPalette1();
	}

	const cb::perMaterialChange& at(unsigned int index) const { return m_materials.at(index); }
	std::vector<cb::perMaterialChange>::const_iterator begin() const { return m_materials.begin(); }
	std::vector<cb::perMaterialChange>::const_iterator end() const { return m_materials.end(); }
	size_t size() const { return m_materials.size(); }
	size_t capacity() const { return m_maxSize; }
	const cb::perMaterialChange* data() const { return m_materials.data(); }

private:
	void push_back(const cb::perMaterialChange& material) {
		assert(m_materials.size() <= m_maxSize && "Cannot exceed max material size as shaders would need to be recompiled");
		m_materials.push_back(material);
	}

	void change(unsigned int index, const cb::perMaterialChange& material) {
		assert(index < m_materials.size() && "Index out of bound");
		m_materials.at(index) = material;
	}

	void loadDefaultPalette1();

private:
	std::vector<cb::perMaterialChange> m_materials;
	unsigned int m_maxSize = 20;

private:
	friend class PaletteGui;
};
