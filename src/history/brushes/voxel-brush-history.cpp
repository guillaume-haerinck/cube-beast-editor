#include "voxel-brush-history.h"

#include "components/physics/transform.h"

VoxelBrushHistory::VoxelBrushHistory(const std::vector<glm::ivec3>& addedPoints) 
    : m_addedPoints(addedPoints) {}

VoxelBrushHistory::~VoxelBrushHistory() {}

void VoxelBrushHistory::undo() {

}

void VoxelBrushHistory::redo() {
    
}
