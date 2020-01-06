#include "rbf-history.h"

#include "components/physics/transform.h"

RBFHistory::RBFHistory(met::registry& registry, const std::vector<glm::ivec3>& unchangedPoints) 
    : m_registry(registry), m_unchangedPoints(unchangedPoints) {}

RBFHistory::~RBFHistory() {}

void RBFHistory::undo() {
    // TODO
}

void RBFHistory::redo() {
    
}
