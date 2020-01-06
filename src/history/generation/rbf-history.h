#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <met/met.hpp>

#include "history/i-history.h"
#include "maths/rbf.h"

class RBFHistory : public IHistory {
public:
    RBFHistory(met::registry& registry, const std::vector<glm::ivec3>& unchangedPoints);
    virtual ~RBFHistory();

    void undo() override;
    void redo() override;

private:
    met::registry& m_registry;
    std::vector<glm::ivec3> m_unchangedPoints;
};
