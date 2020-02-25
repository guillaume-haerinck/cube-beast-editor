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
	const cb::perMaterialChange& getSelected() const { return m_materials.at(m_selectedIndex); }
	std::vector<cb::perMaterialChange>::const_iterator begin() const { return m_materials.begin(); }
	std::vector<cb::perMaterialChange>::const_iterator end() const { return m_materials.end(); }
	size_t size() const { return m_materials.size(); }
	size_t capacity() const { return m_maxSize; }
	const cb::perMaterialChange* data() const { return m_materials.data(); }
	unsigned int selectedIndex() const { return m_selectedIndex; }
	unsigned int byteWidth() const { return sizeof(cb::perMaterialChange) * m_maxSize; }

	bool hasToBeUpdated() const { return m_hasToBeUpdated; }

private:
	void push_back(const cb::perMaterialChange& material) {
		assert(m_materials.size() <= m_maxSize && "Cannot exceed max material size as shaders would need to be recompiled");
		m_materials.push_back(material);
		m_hasToBeUpdated = true;
	}

	void change(unsigned int index, const cb::perMaterialChange& material) {
		assert(index < m_materials.size() && "Index out of bound");
		m_materials.at(index) = material;
		m_hasToBeUpdated = true;
	}

	void loadDefaultPalette1();

private:
	std::vector<cb::perMaterialChange> m_materials;
	unsigned int m_maxSize = 20;
	unsigned int m_selectedIndex = 0;
	bool m_hasToBeUpdated = true;

private:
	friend class PaletteGui;
	friend class RenderSystem;
};
