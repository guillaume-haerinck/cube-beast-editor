#pragma once

#include <glm/glm.hpp>

#include "i-system.h"
#include "context.h"
#include "scomponents/singleton-components.h"

class RenderSystem : public ISystem {
public:
	RenderSystem(Context& context, const SingletonComponents& scomps);
	virtual ~RenderSystem();

	void update() override;

private:
	Context& m_ctx;
	const SingletonComponents& m_scomps;
};
