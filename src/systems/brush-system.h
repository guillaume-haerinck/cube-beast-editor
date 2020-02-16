#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "systems/i-system.h"
#include "context.h"

class BrushSystem : public ISystem {
public:
    BrushSystem();
    virtual ~BrushSystem();

	void update();

private:
    void voxelBrush();
    void boxBrush();

private:
    std::vector<glm::ivec3> m_tempAddedPos;
};
