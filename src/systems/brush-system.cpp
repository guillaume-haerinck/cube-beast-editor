#include "brush-system.h"

#include <profiling/instrumentor.h>
#include <algorithm>

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
        
        break;
    }
        
    case BrushUse::REMOVE:
        break;

    case BrushUse::PAINT:
        break;

    default: break;
    }
}

void BrushSystem::boxBrush() {
    PROFILE_SCOPE("BoxBrush update");

    glm::ivec3 endPos = m_scomps.hovered.position();
    glm::ivec3 startPos;
    if (m_tempAddedPos.size() >= 1)
        startPos = m_tempAddedPos.at(0);
    else
        startPos = endPos;

    if (m_scomps.hovered.isCube()) {
        switch (m_scomps.hovered.face()) {
        case Face::FRONT: endPos.z--; break;
        case Face::BACK: endPos.z++; break;
        case Face::RIGHT: endPos.x++; break;
        case Face::LEFT: endPos.x--; break;
        case Face::TOP: endPos.y++; break;
        case Face::BOTTOM: endPos.y--; break;
        case Face::NONE: break;
        default:
            assert(false && "Unknown hovered face");
        }
    }

    if (startPos.x > endPos.x) { std::swap(startPos.x, endPos.x); }
    if (startPos.y > endPos.y) { std::swap(startPos.y, endPos.y); }
    if (startPos.z > endPos.z) { std::swap(startPos.z, endPos.z); }

    // Fill selection area
    std::vector<glm::ivec3> selectedArea;
    for (size_t x = startPos.x; x <= endPos.x; x++) {
        for (size_t y = startPos.y; y <= endPos.y; y++) {
            for (size_t z = startPos.z; z <= endPos.z; z++) {
                selectedArea.push_back(glm::ivec3(x, y, z));
            }
        }
    }

    switch (m_scomps.brush.usage()) {
    case BrushUse::ADD: {
        PROFILE_SCOPE("BoxBrush add");
        break;
    }

    case BrushUse::REMOVE : {
        PROFILE_SCOPE("BoxBrush remove");
        break;
    }

    case BrushUse::PAINT : {
        PROFILE_SCOPE("BoxBrush paint");
        break;
    }

    default: break;
    }
}
