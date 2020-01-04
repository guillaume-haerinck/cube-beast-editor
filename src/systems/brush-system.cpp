#include "brush-system.h"

#include <profiling/instrumentor.h>

#include "components/physics/transform.h"
#include "components/graphics/material.h"

BrushSystem::BrushSystem(Context& ctx, SingletonComponents& scomps) : m_ctx(ctx), m_scomps(scomps) {}

BrushSystem::~BrushSystem() {}

void BrushSystem::update() {
    PROFILE_SCOPE("BrushSystem update");

    if (!m_scomps.brush.started() && m_tempAddedPos.size() > 0)
        m_tempAddedPos.clear();

    if (m_scomps.hovered.exist() && m_scomps.brush.started()) {
        switch (m_scomps.brush.type()) {
            case BrushType::VOXEL: voxelBrush(); break;
            case BrushType::BOX: boxBrush(); break;
            default: break;
        }
    }
}

void BrushSystem::voxelBrush() {
    PROFILE_SCOPE("VoxelBrush update");    

    comp::Transform trans;
    trans.position = m_scomps.hovered.position();

    for (const glm::ivec3& pos : m_tempAddedPos) {
        if (pos == trans.position) {
            return;
        }
    }

    if (m_scomps.hovered.isCube()) {
        switch (m_scomps.hovered.face()) {
        case Face::FRONT: trans.position.z--; break;
        case Face::BACK: trans.position.z++; break;
        case Face::RIGHT: trans.position.x++; break;
        case Face::LEFT: trans.position.x--; break;
        case Face::TOP: trans.position.y++; break;
        case Face::BOTTOM: trans.position.y--; break;
        case Face::NONE: break;
        default:
            assert(false && "Unknown hovered face");
        }
    }

    switch (m_scomps.brush.usage()) {
    case BrushUse::ADD: {
        met::entity entity = m_ctx.registry.create();
        comp::Material material;
        material.sIndex = m_scomps.materials.selectedIndex();
        m_ctx.registry.assign<comp::Material>(entity, material);
        m_ctx.registry.assign<comp::Transform>(entity, trans);
        m_tempAddedPos.push_back(trans.position);
        break;
    }
        
    case BrushUse::REMOVE:
        if (m_scomps.hovered.isCube())
            m_ctx.registry.destroy(m_scomps.hovered.id());
        break;

    case BrushUse::PAINT:
        if (m_scomps.hovered.isCube()) {
            auto& material = m_ctx.registry.get<comp::Material>(m_scomps.hovered.id());
            material.sIndex = m_scomps.materials.selectedIndex();
        }
        break;

    default: break;
    }
}

void BrushSystem::boxBrush() {
    PROFILE_SCOPE("BoxBrush update");

    
}
