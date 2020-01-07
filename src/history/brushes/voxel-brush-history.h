#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <met/met.hpp>

#include "history/i-history.h"
#include "maths/rbf.h"

class VoxelBrushHistory : public IHistory {
public:
    VoxelBrushHistory (/* args */);
    virtual ~VoxelBrushHistory ();

private:
    met::registry& m_registry;
    std::vector<glm::ivec3> m_addedPoints;
};
