#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "history/i-history.h"
#include "maths/rbf.h"

class VoxelBrushHistory : public IHistory {
public:
    VoxelBrushHistory(const std::vector<glm::ivec3>& addedPoints);
    virtual ~VoxelBrushHistory();

    virtual void undo() override;
    virtual void redo() override;

private:
    std::vector<glm::ivec3> m_addedPoints;
};
