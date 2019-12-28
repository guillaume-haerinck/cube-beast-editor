#pragma once

#include <vector>
#include <glm/glm.hpp>

/**
 * @brief Structure of data for one PBR material
 */
struct Material {
	glm::vec3 albedo;
};

class Materials {
public:
	Materials() {}

	const Material& at(unsigned int index) const { return m_materials.at(index); }
	std::vector<Material>::const_iterator begin() const { return m_materials.begin(); }
	std::vector<Material>::const_iterator end() const { return m_materials.end(); }
	size_t size() const { return m_materials.size(); }

private:
	void push_back(const Material& material) { m_materials.push_back(material); }

	void change(unsigned int index, const Material& material) {
		assert(index < m_materials.size() && "Material index do not exist !");
		m_materials.at(index) = material;
	}

private:
	std::vector<Material> m_materials;

private:
	friend class PaletteGui;
};
