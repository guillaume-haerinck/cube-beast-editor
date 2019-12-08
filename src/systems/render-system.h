#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "i-system.h"
#include "context.h"
#include "scomponents/singleton-components.h"
#include "components/physics/transform.h"

class RenderSystem : public ISystem {
public:
	RenderSystem(Context& context, SingletonComponents& scomps);
	virtual ~RenderSystem();

	void update() override;

private:
	Context& m_ctx;
	SingletonComponents& m_scomps;
	std::vector<glm::vec3> m_tempTranslations;
	std::vector<met::entity> m_tempEntityIds;
	met::entity lastSelect;
};
