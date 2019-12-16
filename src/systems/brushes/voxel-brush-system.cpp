#include "voxel-brush-system.h"

#include <debug_break/debug_break.h>

#include "components/physics/transform.h"
#include "components/graphics/material.h"

VoxelBrushSystem::VoxelBrushSystem(Context& ctx, SingletonComponents& scomps) : m_ctx(ctx), m_scomps(scomps) {}

VoxelBrushSystem::~VoxelBrushSystem() {}

void VoxelBrushSystem::update() {
    if (m_scomps.hovered.exist && m_scomps.inputs.actionState.at(scomp::InputAction::BRUSH_VOX_ADD)) {
        comp::Transform trans;
        comp::Material material;
        trans.position = m_scomps.hovered.position;
        met::entity entity = m_ctx.registry.create();
        m_ctx.registry.assign<comp::Material>(entity, material);

        switch (m_scomps.hovered.face) {
            case scomp::Face::FRONT: trans.position.z--; break;
            case scomp::Face::BACK: trans.position.z++; break;
            case scomp::Face::RIGHT: trans.position.x++; break;
            case scomp::Face::LEFT: trans.position.x--; break;
            case scomp::Face::TOP: trans.position.y++; break;
            case scomp::Face::BOTTOM: trans.position.y--; break;  
            default:
                debug_break();
                assert(false && "Unknown hovered face");
        }
        m_ctx.registry.assign<comp::Transform>(entity, trans);
    }
}
