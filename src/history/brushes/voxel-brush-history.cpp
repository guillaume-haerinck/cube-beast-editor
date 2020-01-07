#include "voxel-brush-history.h"

#include "components/physics/transform.h"

VoxelBrushHistory::VoxelBrushHistory(met::registry& registry, const std::vector<glm::ivec3>& addedPoints) 
    : m_registry(registry), m_addedPoints(addedPoints) {}

VoxelBrushHistory::~VoxelBrushHistory() {}

void VoxelBrushHistory::undo() {
    m_registry.view<comp::Transform>().each([&](met::entity id, comp::Transform& transform){
        // TODO
    });
}

void VoxelBrushHistory::redo() {
    
}
