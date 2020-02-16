#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "history/i-history.h"
#include "maths/rbf.h"

class RBFHistory : public IHistory {
public:
    RBFHistory(const std::vector<glm::ivec3>& unchangedPoints);
    virtual ~RBFHistory();

    void undo() override;
    void redo() override;

private:
    std::vector<glm::ivec3> m_unchangedPoints;
};
