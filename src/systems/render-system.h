#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "i-system.h"

class RenderSystem : public ISystem {
public:
	RenderSystem();
	virtual ~RenderSystem();

	void update();

private:
	void updateCBperNiMesh_facePlane();
	void updateCBperNiMesh(glm::vec3 translation, float scale, glm::vec3 albedo);

private:
	std::vector<glm::vec3> m_tempTranslations;
	std::vector<glm::vec3> m_tempEntityIds;
	std::vector<unsigned int> m_tempMaterialIds;
};
