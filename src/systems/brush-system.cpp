#include "brush-system.h"

#include "components/physics/transform.h"
#include "components/graphics/material.h"

BrushSystem::BrushSystem(Context& ctx, SingletonComponents& scomps) : m_ctx(ctx), m_scomps(scomps) {}

BrushSystem::~BrushSystem() {}

void BrushSystem::update() {
    if (m_scomps.hovered.exist && m_scomps.isBrushStarted) {
        comp::Transform trans;
        comp::Material material;
        trans.position = m_scomps.hovered.position;

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

        met::entity entity = m_ctx.registry.create();
        m_ctx.registry.assign<comp::Material>(entity, material);
        m_ctx.registry.assign<comp::Transform>(entity, trans);

        // temp
        m_scomps.isBrushStarted = false;
    }
}
