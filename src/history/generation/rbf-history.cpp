#include "rbf-history.h"

RBFHistory::RBFHistory(const std::vector<glm::ivec3>& unchangedPoints) 
    : m_unchangedPoints(unchangedPoints) {}

RBFHistory::~RBFHistory() {}

void RBFHistory::undo() {
    // TODO
}

void RBFHistory::redo() {
    
}
