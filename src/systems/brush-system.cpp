#include "brush-system.h"

#include "components/physics/transform.h"
#include "components/graphics/material.h"

BrushSystem::BrushSystem(Context& ctx, SingletonComponents& scomps) : m_ctx(ctx), m_scomps(scomps) {}

BrushSystem::~BrushSystem() {}

void BrushSystem::update() {
    if (!m_scomps.isBrushStarted && m_tempAddedPos.size() > 0)
        m_tempAddedPos.clear();

    if (m_scomps.hovered.exist && m_scomps.isBrushStarted) {
        switch (m_scomps.currentBrush) {
            case BrushType::VOXEL: voxelBrush(); break;
            default: break;
        }
    }
}

void BrushSystem::voxelBrush() {
    comp::Transform trans;
    comp::Material material;
    trans.position = m_scomps.hovered.position;

    for (const glm::ivec3& pos : m_tempAddedPos) {
        if (pos == trans.position) {
            return;
        }
    }

    if (m_scomps.hovered.isCube) {
        switch (m_scomps.hovered.face) {
        case scomp::Face::FRONT: trans.position.z--; break;
        case scomp::Face::BACK: trans.position.z++; break;
        case scomp::Face::RIGHT: trans.position.x++; break;
        case scomp::Face::LEFT: trans.position.x--; break;
        case scomp::Face::TOP: trans.position.y++; break;
        case scomp::Face::BOTTOM: trans.position.y--; break;
        case scomp::Face::NONE: break;
        default:
            assert(false && "Unknown hovered face");
        }
    }

    switch (m_scomps.currentBushUse) {
    case BrushUse::ADD : {
        met::entity entity = m_ctx.registry.create();
        m_ctx.registry.assign<comp::Material>(entity, material);
        m_ctx.registry.assign<comp::Transform>(entity, trans);
        m_tempAddedPos.push_back(trans.position);
        break;
    }
        
    case BrushUse::REMOVE : {
        if (m_scomps.hovered.isCube)
            m_ctx.registry.destroy(m_scomps.hovered.id);
        break;
    }

    default: break;
    }
}
