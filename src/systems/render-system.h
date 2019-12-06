#pragma once

#include <glm/glm.hpp>

#include "i-system.h"
#include "core/context.h"
#include "components/physics/position.h"
#include "components/graphics/mesh.h"
#include "components/graphics/pipeline.h"

class RenderSystem : public ISystem {
public:
	RenderSystem(Context& context);
	virtual ~RenderSystem();

	void update() override;

private:
	Context& m_ctx;
};
