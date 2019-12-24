#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "systems/i-system.h"
#include "context.h"

class BrushSystem : public ISystem {
public:
    BrushSystem(Context& ctx, SingletonComponents& scomps);
    virtual ~BrushSystem();

	void update() override;

private:
    void voxelBrush();

private:
    Context& m_ctx;
    SingletonComponents& m_scomps;
    std::vector<glm::ivec3> m_tempAddedPos;
};
