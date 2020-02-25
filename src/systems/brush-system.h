#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "systems/i-system.h"
#include "context.h"
#include "data/io/brush.h"
#include "data/io/hovered.h"

class BrushSystem : public ISystem {
public:
    BrushSystem();
    virtual ~BrushSystem();

	void update(const Brush& brush, const Hovered& hovered);

private:
    void voxelBrush(const Brush& brush, const Hovered& hovered);
    void boxBrush(const Brush& brush, const Hovered& hovered);

private:
    std::vector<glm::ivec3> m_tempAddedPos;
};
