#include "voxel-brush-history.h"

#include "components/physics/transform.h"

VoxelBrushHistory::VoxelBrushHistory(met::registry& registry, const std::vector<glm::ivec3>& unchangedPoints) 
    : m_registry(registry), m_unchangedPoints(unchangedPoints) {}

VoxelBrushHistory::~VoxelBrushHistory() {}

void VoxelBrushHistory::undo() {
    m_ctx.registry.view<comp::Transform>().each([&](met::entity id, comp::Transform& transform){
        // TODO
    });
}

void VoxelBrushHistory::redo() {
    
}
