#pragma once

#include <vector>
#include <glm/glm.hpp>

/**
 * @brief Structure of data for one PBR material
 */
struct Material {
	glm::vec3 albedo;
	float emissiveFactor;
};

class Materials {
public:
	Materials() {}

	const Material& at(unsigned int index) const { return m_materials.at(index); }
	std::vector<Material>::const_iterator begin() const { return m_materials.begin(); }
	std::vector<Material>::const_iterator end() const { return m_materials.end(); }
	size_t size() const { return m_materials.size(); }

private:
	std::vector<Material> m_materials;

private:
	friend class PaletteGui;
};
